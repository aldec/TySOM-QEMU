#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

source ./config.sh

if [ ! -d hardware ]; then
    mkdir -p hardware
fi

cd hardware

$VIVADO_PATH/bin/vivado -mode batch -source ../scripts/project_1.tcl -tclargs --project_name ${hw_project_name}
