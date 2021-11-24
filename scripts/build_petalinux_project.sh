#!/bin/bash

if [ -z ${PETALINUX+x} ]
then
   echo 'could not find Petalinux path $PETALINUX'
   echo 'did you source Petalinux settings.sh ?'
   exit 1
fi

petalinux-create --type project --template zynq --name $project_name
cd $project_name
petalinux-config --get-hw-description=./../example_hw --silentconfig
petalinux-build

