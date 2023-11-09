#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

echo $PETALINUX_PATH
$PETALINUX_PATH/components/yocto/buildtools_extended/sysroots/x86_64-petalinux-linux/usr/bin/qemu-system-microblazeel \
-M microblaze-fdt \
-serial /dev/null \
-serial mon:stdio \
-display none \
-kernel ./$project_name/images/linux/pmu_rom_qemu_sha3.elf \
-device loader,file=./$project_name/images/linux/pmufw.elf \
-hw-dtb ./$project_name/images/linux/zynqmp-qemu-multiarch-pmu.dtb \
-machine-path ./$project_name/qemu_cosim/tmp \
-device loader,addr=0xfd1a0074,data=0x1011003,data-len=4 \
-device loader,addr=0xfd1a007C,data=0x1010f03,data-len=4 &

sleep 3

$PETALINUX_PATH/components/yocto/buildtools_extended/sysroots/x86_64-petalinux-linux/usr/bin/qemu-system-aarch64 \
-M arm-generic-fdt \
-serial /dev/null \
-serial mon:stdio \
-display none \
-device loader,file=./$project_name/images/linux/bl31.elf,cpu-num=0 \
-device loader,file=./$project_name/images/linux/rootfs.cpio.gz.u-boot,addr=0x04000000,force-raw=on \
-device loader,file=./$project_name/images/linux/u-boot.elf \
-device loader,file=./$project_name/images/linux/Image,addr=0x00200000,force-raw=on \
-device loader,file=./$project_name/images/linux/system.dtb,addr=0x00100000,force-raw=on \
-device loader,file=./$project_name/images/linux/boot.scr,addr=0x20000000,force-raw=on \
-gdb tcp::9000 \
-net nic \
-net nic \
-net nic,netdev=eth0 \
-netdev user,id=eth0,tftp=/tftpboot,hostfwd=tcp::1534-:1534,hostfwd=tcp::5555-:22 \
-net nic \
-hw-dtb ./src/qemu/zcu102-arm.cosim.multi_2023.1.dtb \
-sync-quantum $SYNC_QUANTUM \
-machine-path ./$project_name/qemu_cosim/tmp \
-global xlnx,zynqmp-boot.cpu-num=0 \
-global xlnx,zynqmp-boot.use-pmufw=true \
-m 4G


#-hw-dtb ./src/qemu/zcu102-arm.cosim.multi_2022.1.dtb \
#-netdev user,id=eth0,tftp=/tftpboot,hostfwd=tcp::1534-:1534,hostfwd=tcp::5555-:22 \
#-kernel ./src/qemu/pmu_rom_qemu_sha3.elf \
# -hw-dtb ./src/qemu/zynqmp-qemu-multiarch-arm.dtb
# -hw-dtb ./src/qemu/zcu102-arm.cosim.multi.dtb \
#-icount 1 
#board-zynqmp-cosim-zcu104.dtb
#zcu102-arm.cosim.single_2023.1.dtb
#-global xlnx,zynqmp-boot.use-pmufw=true \
