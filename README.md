# SystemC_Qemu_CoSim
1. Requirements
- Vivado 2021.1
- Petalinux 2021.1
- Riviera-Pro 2021.10

2. Simulator Library
- Generate Xilinx simulation libraries using tutorial below:
 
https://www.aldec.com/en/support/resources/documentation/articles/1710

3. Preparing co-simulation files
- **Modify config.sh script. Set the proper paths to Riviera-Pro simulator and Xilinx Petalinux tool.**
- Run config.sh script

`./config.sh`
- Run prepare_files.sh script (this may take around 30 minutes for the petalinux project to be built) 

`./prepare_files.sh`

4. Running co-simulation

`./run_example_cosim.sh`
- The QEMU process should appear in a new terminal window, and in the meantime, the co-simulation in Riviera should begin. 



