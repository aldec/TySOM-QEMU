#!/bin/bash

if [ -z ${PETALINUX+x} ]
then
   echo 'could not find Petalinux path $PETALINUX'
   echo 'did you source Petalinux settings.sh ?'
   exit 1
fi

_NEW=0
if [ ! -e ${project_name} ]
then
    petalinux-create --type project --template zynq --name $project_name
    _NEW=1
fi

pushd $project_name
petalinux-config --get-hw-description=./../hardware --silentconfig

if [ ${_NEW} == 1 ]
then
    cp -rf ../scripts/petalinux/project-spec/ ./
fi 

sed -i 's:# CONFIG_imagefeature-debug-tweaks is not set:CONFIG_imagefeature-debug-tweaks=y:g' project-spec/configs/rootfs_config
sed -i 's:# CONFIG_imagefeature-empty-root-password is not set:CONFIG_imagefeature-empty-root-password=y:g' project-spec/configs/rootfs_config
sed -i 's:# CONFIG_imagefeature-serial-autologin-root is not set:CONFIG_imagefeature-serial-autologin-root=y:g' project-spec/configs/rootfs_config

petalinux-build
popd
