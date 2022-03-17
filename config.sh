#!/bin/bash

# Set the proper paths below
#(e.g. /$TOOLS_DIR/Aldec/Riviera-PRO-2021.10-x64)
#(e.g. /$TOOLS_DIR/Xilinx/Linux/petalinux-v2021.1)
#(e.g. /$TOOLS_DIR/Xilinx/Vivado/2021.1)
export RIVIERA_PATH=/Riviera-PRO-2021.10-x64
export PETALINUX_PATH=/petalinux-v2021.1
export VIVADO_PATH=/Vivado/2021.1
export project_name=tysom1_petalinux


# Path to Qemu Socket
RPORT_PATH=`pwd`/${project_name}/qemu_cosim/qemu-rport-_cosim@0
export RPORT_PATH_SC=${RPORT_PATH}
export RPORT_PATH_HDL=unix:${RPORT_PATH}
