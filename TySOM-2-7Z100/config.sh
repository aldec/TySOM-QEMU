#!/bin/bash

# Set the proper paths below
#(e.g. /$TOOLS_DIR/Aldec/Riviera-PRO-2023.04-x64)
#(e.g. /$TOOLS_DIR/Xilinx/Linux/petalinux-v2023.1)
#(e.g. /$TOOLS_DIR/Xilinx/Vivado/2023.1)

export RIVIERA_PATH=
export PETALINUX_PATH=
export VIVADO_PATH=

export hw_project_name=project_1
export hw_design_name=design_1
export project_name=tysom2_petalinux

export SYNC_QUANTUM=100000
RPORT_PATH=../${project_name}/qemu_cosim/tmp/qemu-rport-_cosim@0
export RPORT_PATH_SC=${RPORT_PATH}
export RPORT_PATH_HDL=unix:${RPORT_PATH}
