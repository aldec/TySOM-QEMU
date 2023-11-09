#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

source ./config.sh
source $PETALINUX_PATH/settings.sh $PETALINUX_PATH

./scripts/build_petalinux_project.sh
