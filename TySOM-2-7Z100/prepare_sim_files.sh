#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

source ./config.sh

if [ ! -d riviera ]; then
    mkdir -p riviera
fi

cp -f src/make_wv.do riviera
cp -f src/virt_stat.xml riviera
cp -f hardware/${hw_project_name}/${hw_project_name}.sim/sim_1/behav/riviera/${hw_design_name}_wrapper_compile.do riviera
cp -f hardware/${hw_project_name}/${hw_project_name}.sim/sim_1/behav/riviera/${hw_design_name}_wrapper_simulate.do riviera
cp -f src/run.sh riviera

sed -i 's:../../../../:../hardware/$env(hw_project_name)/:g' ./riviera/${hw_design_name}_wrapper_compile.do
sed -i 's:glbl.v:../src/glbl.v:g' ./riviera/${hw_design_name}_wrapper_compile.do
sed -i '/quit -force/d' ./riviera/${hw_design_name}_wrapper_compile.do
sed -i '/transcript off/d' ./riviera/${hw_design_name}_wrapper_compile.do
sed -i '/transcript on/d' ./riviera/${hw_design_name}_wrapper_compile.do
sed -i 's:/[[:alnum:]/]\+/Xilinx/Vivado/2023\.1:\$env(VIVADO_PATH):g' ./riviera/${hw_design_name}_wrapper_compile.do
sed -i '/transcript off/d' ./riviera/${hw_design_name}_wrapper_simulate.do
sed -i '/transcript on/d' ./riviera/${hw_design_name}_wrapper_simulate.do
sed -i '/onbreak {quit -force}/d' ./riviera/${hw_design_name}_wrapper_simulate.do
sed -i s':onerror {quit -force}:set GENERIC_PATH /$env(hw_design_name)_wrapper/$env(hw_design_name)_i/processing_system7_0/QEMU_PATH_TO_SOCKET_G \
echo $GENERIC_PATH \
set QUANTUM_GENERIC_PATH /$env(hw_design_name)_wrapper/$env(hw_design_name)_i/processing_system7_0/QEMU_SYNC_QUANTUM_G \
echo \$QUANTUM_GENERIC_PATH \
set SIM_OPT \"-ieee_nowarn -i 5000 -t 1ps -relax +notimingchecks +access +r \":' ./riviera/${hw_design_name}_wrapper_simulate.do
sed -i s'/asim +access +r/eval asim $SIM_OPT/' ./riviera/${hw_design_name}_wrapper_simulate.do
sed -i s'/+m+design_1_wrapper/+m+design_1_wrapper+axi_monitor/' ./riviera/${hw_design_name}_wrapper_simulate.do
sed -i '17,$d' ./riviera/${hw_design_name}_wrapper_simulate.do
sed -i s'/xil_defaultlib.glbl/xil_defaultlib.glbl work.axi_monitor \\\
-g$GENERIC_PATH=$env(RPORT_PATH_HDL) -g$QUANTUM_GENERIC_PATH=$env(SYNC_QUANTUM) \n \
do make_wv.do \n \
run -all/' ./riviera/${hw_design_name}_wrapper_simulate.do

python3 ./scripts/generate_sim_files.py  hardware/${hw_project_name}/${hw_project_name}.gen/sources_1/bd/${hw_design_name}/ip/${hw_design_name}_processing_system7_0_0/${hw_design_name}_processing_system7_0_0_stub.vhdl riviera/

sed -i 's:vcom -work xil_defaultlib -2008 ./../src/design_1_processing_system7_0_0.vhd:vcom -work xil_defaultlib -2008 ./../src/design_1_processing_system7_0_0.vhd \
vlog -sv -incr -l xilinx_vip -work work "../src/testbench.sv":' ./riviera/zynq7_compile_cosim.do
