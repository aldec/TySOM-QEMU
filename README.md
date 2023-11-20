# SystemC QEMU Co-simulation

1. Requirements
   - Vivado 2023.1
   - Petalinux 2023.1
   - Riviera-Pro 2023.04
   
2. Simulator Library
   - Generate Xilinx simulation libraries (unisim, unisims_ver, unimacro_ver, secureip) using tutorial below:

        https://www.aldec.com/en/support/resources/documentation/articles/1710
     
     or 
     
    - Get pre-compiled libraries from
     
        https://www.aldec.com/en/downloads (an account is required)

3. Board selection `TySOM-2-7Z100` or `TySOM-3-ZU7`:

   ```sh
   cd TySOM-2-7Z100 
   ```
   or
   ```sh
   cd TySOM-3-ZU7 
   ```

4. Preparing co-simulation files
   1. **Modify config.sh script. Set the proper paths to Riviera-Pro simulator and Xilinx Petalinux tool.**
   2. Run scripts

     ```sh
   ./prepare_hardware.sh
   ./prepare_linux.sh
   ./prepare_sim_files.sh
   ```

5. Running co-simulation

   **NOTE**: Make sure if simulation library for Riviera-Pro simulator is ready
   to use. If not, please rebuild the library under Vivado with Riviera-Pro.
   Check if path to the library for simulation is set correctly in the
   `riviera/design_1_wrapper_compile.do` file:

   ```tcl
   amap -link /path/to/generated/libraries/library.cfg
   ```
   or 
   ```tcl
   amap secureip /path/to/secureip.lib
   amap unisim /path/to/unisim.lib
   amap unisims_ver /path/to/unisims_ver.lib
   amap unimacro_ver /path/to/unimacro_ver.lib
   ```

   `./run_example_cosim.sh`

   - The QEMU process should appear in a new terminal window, and in the meantime, the co-simulation in Riviera should begin.

## Limitations:
- Windows is not supported.
- The supported platforms:
  - Red Hat Enterprise Workstation/Server 7.4, 7.5, 7.6, 7.7, 7.8, 7.9, 8.1, 8.2 (64-bit)
  - CentOS Workstation/Server 7.4, 7.5, 7.6, 7.7, 7.8, 7.9, 8.1, 8.2 (64-bit)
  - Ubuntu Linux Workstation/Server 16.04.5, 16.04.6, 18.04.1, 18.04.2, 18.04.3,
    18.04.4,18.04.5, 20.04, 20.04.1 (64-bit)
- The example has been verified to work with operating systems: CentOS 7.x and
  Ubuntu 20.04.
