#!/bin/bash


$PETALINUX_PATH/components/yocto/buildtools_extended/sysroots/x86_64-petalinux-linux/usr/bin/qemu-system-aarch64 \
-M arm-generic-fdt-7series -machine linux=on   -serial /dev/null -serial mon:stdio -display none \
-kernel ./$project_name/images/linux/zImage -initrd ./$project_name/images/linux/rootfs.cpio.gz.u-boot -gdb tcp::9000 \
-dtb ./$project_name/images/linux/system.dtb  -net nic,netdev=eth0 -netdev user,id=eth0,tftp=/tftpboot -net nic \
-device loader,addr=0xf8000008,data=0xDF0D,data-len=4 -device loader,addr=0xf8000140,data=0x00500801,data-len=4 \
-device loader,addr=0xf800012c,data=0x1ed044d,data-len=4 -device loader,addr=0xf8000108,data=0x0001e008,data-len=4 \
-device loader,addr=0xF8000910,data=0xF,data-len=0x4  -hw-dtb ./$project_name/qemu_cosim/qemu_hw_system.dtb -machine-path ./$project_name/qemu_cosim -icount 1 -sync-quantum 10000
