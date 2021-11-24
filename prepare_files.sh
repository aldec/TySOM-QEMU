#!/bin/bash

source ./config.sh

source $RIVIERA_PATH/etc/setenv
source $RIVIERA_PATH/etc/setgcc
source $PETALINUX_PATH/settings.sh

if [ ! -L ./src/libsystemctlm-soc ] ; then
   ln -s ../backup/libsystemctlm-soc ./src/
fi

if [ ! -L ./src/src_sc ] ; then
   ln -s ../backup/src_sc ./src/
fi

if [ ! -L ./src/src_vhdl ] ; then
   ln -s ../backup/src_vhdl ./src/
fi

./scripts/build_petalinux_project.sh
./scripts/generate_qemu_device_tree_zynq7.sh ./$project_name
python3 ./scripts/generate_sim_files.py src/project_1.gen/sources_1/bd/design_1/ip/design_1_processing_system7_0_0/design_1_processing_system7_0_0_stub.vhdl riviera/
