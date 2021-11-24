#!/bin/bash

source ./config.sh

#cleanup
if [ -d $project_name ] 
then
   rm -rf $project_name
fi

rm -rf riviera/riviera riviera/compile riviera/*.log riviera/*.cfg riviera/work riviera/zynq7_compile_cosim.do riviera/all.do riviera/*.asdb riviera/*.o riviera/*.so
