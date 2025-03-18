#!/bin/bash
set -euo pipefail
#set -euxo pipefail
IFS=$'\n\t'

source ./config.sh

rm -rf ./$project_name/qemu_cosim/tmp/
mkdir -p ./$project_name/qemu_cosim/tmp/

source $PETALINUX_PATH/settings.sh

xterm -e ./scripts/run_qemu.sh > qemu_log.txt 2>&1 &
cd riviera
./run.sh
cd ..
