#!/bin/bash

source ./config.sh
rm -rf ${project_name}/qemu_cosim/tmp
mkdir -p ${project_name}/qemu_cosim/tmp
  
source $PETALINUX_PATH/settings.sh

xterm -e ./scripts/run_qemu.sh &
cd riviera
./run.sh
cd ..
