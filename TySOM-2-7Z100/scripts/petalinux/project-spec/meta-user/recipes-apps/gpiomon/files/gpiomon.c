#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>
#include <signal.h>
#include <poll.h>

// AXI GPIO register offsets based on Xilinx PG144 documentation
#define GPIO_DATA_OFFSET      0x00    // Channel 1 Data Register
#define GPIO_TRI_OFFSET       0x04    // Channel 1 Tri-state (Direction) Register
#define GPIO_GIER_OFFSET      0x11C   // Global Interrupt Enable Register
#define GPIO_IPEIR_OFFSET     0x128   // IP Interrupt Enable Register (IER)
#define GPIO_IPISR_OFFSET     0x120   // IP Interrupt Status Register (Clear interrupts)

// Hardware physical address and address range size
#define GPIO_BASE_ADDR        0x41200000
#define GPIO_SIZE             0x10000

/* Global file descriptor for UIO device */
int fds_raw = -1;
/* Atomic flag for graceful termination via signals */
volatile sig_atomic_t keep_running = 1;

/**
 * Signal handler to catch Ctrl+C or kill commands
 */
void sigint_handler(int sig) {
    keep_running = 0;
}

/**
 * Structure to hold the file descriptor for /dev/mem and virtual memory mapping
 */
typedef struct {
    void* base;
    int fd;
} axi_gpio_linux_t;

/**
 * Maps physical GPIO memory to user-space virtual memory using /dev/mem
 */
int axi_gpio_linux_init(axi_gpio_linux_t* gpio) {
    // Open memory device with O_SYNC to bypass CPU caching
    gpio->fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (gpio->fd < 0) {
        perror("open /dev/mem");
        return -1;
    }
    
    // Map the physical AXI address to a virtual pointer
    gpio->base = mmap(NULL, GPIO_SIZE, PROT_READ | PROT_WRITE, 
                      MAP_SHARED, gpio->fd, GPIO_BASE_ADDR);
    
    if (gpio->base == MAP_FAILED) {
        perror("mmap GPIO");
        close(gpio->fd);
        return -1;
    }
    
    printf("AXI GPIO mapped at virtual address: %p (Physical: 0x%08lx)\n", 
           gpio->base, (uintptr_t)GPIO_BASE_ADDR);
    return 0;
}

/**
 * Cleanup: Unmap memory and close file descriptors
 */
void axi_gpio_linux_close(axi_gpio_linux_t* gpio) {
    if (gpio->base != MAP_FAILED) {
        munmap(gpio->base, GPIO_SIZE);
    }
    if (gpio->fd >= 0) {
        close(gpio->fd);
    }
}

/**
 * Standard volatile write for memory-mapped registers
 */
void axi_gpio_linux_write32(axi_gpio_linux_t* gpio, uint32_t offset, uint32_t value) {
    volatile uint32_t* reg = (volatile uint32_t*)((uintptr_t)gpio->base + offset);
    *reg = value;
}

/**
 * Standard volatile read for memory-mapped registers
 */
uint32_t axi_gpio_linux_read32(axi_gpio_linux_t* gpio, uint32_t offset) {
    volatile uint32_t* reg = (volatile uint32_t*)((uintptr_t)gpio->base + offset);
    return *reg;
}

/**
 * Acknowledges the interrupt in the AXI GPIO IP Core.
 * Writing '1' to the IPISR bit clears the pending interrupt.
 */
void axi_gpio_clear_status(axi_gpio_linux_t* gpio) {
    uint32_t status = axi_gpio_linux_read32(gpio, GPIO_IPISR_OFFSET);
    if (status & 0x00000001) {
        // Toggle-on-write (Write-1-to-clear) as per Xilinx spec
        axi_gpio_linux_write32(gpio, GPIO_IPISR_OFFSET, 0x00000001);
    }
}

/**
 * Configures the GPIO direction and sets up interrupt registers.
 */
void axi_gpio_configure(axi_gpio_linux_t* gpio) {
    // Set first 4 pins as inputs (0x0000000F), others as outputs
    axi_gpio_linux_write32(gpio, GPIO_TRI_OFFSET, 0x0000000F);
    
    // Reset/Disable interrupts before enabling
    axi_gpio_linux_write32(gpio, GPIO_IPEIR_OFFSET, 0x00000000); // Disable IP interrupts
    axi_gpio_linux_write32(gpio, GPIO_GIER_OFFSET, 0x00000000);  // Disable Global interrupts
    axi_gpio_linux_write32(gpio, GPIO_IPISR_OFFSET, 0xFFFFFFFF); // Clear pending interrupts (Write-1-to-clear)
    
    // Enable Channel 1 interrupts in the IER register
    axi_gpio_linux_write32(gpio, GPIO_IPEIR_OFFSET, 0x00000001); 
    
    // Enable Global Interrupts (Master switch - Bit 31)
    axi_gpio_linux_write32(gpio, GPIO_GIER_OFFSET, 0x80000000);
}

/**
 * Disables interrupts in the AXI GPIO IP Core.
 * This function performs two steps:
 * 1. Disables the specific channel interrupt in the IER (IP Interrupt Enable Register).
 * 2. Disables the Master Switch in the GIER (Global Interrupt Enable Register).
 */
void axi_gpio_interrupt_disable(axi_gpio_linux_t* gpio) {
    // 1. Disable Channel 1 interrupts in the IER
    // Writing 0x0 stops the IP from generating interrupt signals for this channel.
    axi_gpio_linux_write32(gpio, GPIO_IPEIR_OFFSET, 0x00000000);
    
    // 2. Disable Global Interrupts (Master Switch)
    // Bit 31 of GIER controls whether any interrupt signal leaves the IP core.
    axi_gpio_linux_write32(gpio, GPIO_GIER_OFFSET, 0x00000000);

    // 3. Clear any remaining pending interrupts in the Status Register (IPISR)
    // This ensures no "stale" interrupts are left active in the hardware.
    axi_gpio_linux_write32(gpio, GPIO_IPISR_OFFSET, 0xFFFFFFFF);
    
    printf("GPIO Interrupts disabled and status cleared.\n");
}

int main(int argc, char *argv[]) {
    uint32_t irq_count = 0;
    const uint32_t unmask = 1;
    struct pollfd poll_fds;
    char uio_path[] = "/dev/uio0";
    axi_gpio_linux_t gpio = { .fd = -1, .base = MAP_FAILED };
    
    // Initialize mmap for direct register access
    if (axi_gpio_linux_init(&gpio) < 0) {
        fprintf(stderr, "Fatal: GPIO hardware mapping failed\n");
        return EXIT_FAILURE;
    }
    
    axi_gpio_configure(&gpio);
    
    printf("UIO Interrupt Test Started\n");
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);
    
    // Open UIO device for interrupt handling
    fds_raw = open(uio_path, O_RDWR);
    if (fds_raw < 0) {
        fprintf(stderr, "Error: Could not open %s. Check permissions (sudo).\n", uio_path);
        axi_gpio_linux_close(&gpio);
        return EXIT_FAILURE;
    }

    // Crucial: Initial unmask in UIO driver to enable the GIC line
    write(fds_raw, &unmask, sizeof(unmask));

    // Setup poll structure to monitor UIO device for 'Read Ready' event
    poll_fds.fd = fds_raw;
    poll_fds.events = POLLIN;
    
    printf("Device %s is ready. Waiting for hardware events...\n", uio_path);

    // Initial clear of any stale status in the IP Core
    axi_gpio_clear_status(&gpio);

    while (keep_running) {
        // Wait for interrupt with 500ms timeout
        int ret = poll(&poll_fds, 1, 500); 

        if (ret > 0) {
            if (poll_fds.revents & POLLIN) {
                // Read the interrupt count (this blocks until kernel confirms IRQ)
                if (read(fds_raw, &irq_count, sizeof(irq_count)) > 0) {
                    printf("[IRQ] Event detected! System total: %u\n", irq_count);
                }
                // read data
                uint32_t data = axi_gpio_linux_read32(&gpio, GPIO_DATA_OFFSET);
                printf("DataIn: %x\n", data);
                // 1. Clear status in IP Core (Hardware level)
                axi_gpio_clear_status(&gpio);
                
                // 2. Unmask UIO in Kernel (GIC level)
                write(fds_raw, &unmask, sizeof(unmask));
            }
        } else if (ret == 0) {
            // Optional: Log timeout or perform idle tasks
            // printf("Poll timeout: No interrupt detected.\n");
        } else {
            if (keep_running) perror("Poll error");
        }
    }

    printf("\nGraceful shutdown. Releasing resources...\n");
    close(fds_raw);
    axi_gpio_interrupt_disable(&gpio);
    axi_gpio_linux_close(&gpio);

    return EXIT_SUCCESS;
}
