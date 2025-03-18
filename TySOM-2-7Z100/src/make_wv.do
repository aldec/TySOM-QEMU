am.mappings.load virt_stat.xml

wave /$env(hw_design_name)_wrapper/leds* 

set S_PATH /$env(hw_design_name)_wrapper/$env(hw_design_name)_i

#wave /$env(hw_design_name)_wrapper/$env(hw_design_name)_i/axi_gpio_1/* 

wave -varray "MASTER_0"\
(-vgroup CLK \
(-label "CLK    " ${S_PATH}/processing_system7_0_FCLK_CLK0        ))\
(-vgroup AW \
(-label "AWADDR " ${S_PATH}/axi_interconnect_0_M01_AXI_AWADDR     )\
(-label "AWREADY" ${S_PATH}/axi_interconnect_0_M01_AXI_AWREADY    )\
(-label "AWVALID" ${S_PATH}/axi_interconnect_0_M01_AXI_AWVALID    ))\
(-vgroup W \
(-label "WDATA  " ${S_PATH}/axi_interconnect_0_M01_AXI_WDATA      )\
(-label "WREADY " ${S_PATH}/axi_interconnect_0_M01_AXI_WREADY     )\
(-label "WSTRB  " ${S_PATH}/axi_interconnect_0_M01_AXI_WSTRB      )\
(-label "WVALID " ${S_PATH}/axi_interconnect_0_M01_AXI_WVALID     ))\
(-vgroup B \
(-label "BREADY " ${S_PATH}/axi_interconnect_0_M01_AXI_BREADY     )\
(-label "BRESP  " ${S_PATH}/axi_interconnect_0_M01_AXI_BRESP      )\
(-label "BVALID " ${S_PATH}/axi_interconnect_0_M01_AXI_BVALID     ))\
(-vgroup AR \
(-label "ARADDR " ${S_PATH}/axi_interconnect_0_M01_AXI_ARADDR     )\
(-label "ARREADY" ${S_PATH}/axi_interconnect_0_M01_AXI_ARREADY    )\
(-label "ARVALID" ${S_PATH}/axi_interconnect_0_M01_AXI_ARVALID    ))\
(-vgroup R \
(-label "RDATA  " ${S_PATH}/axi_interconnect_0_M01_AXI_RDATA      )\
(-label "RREADY " ${S_PATH}/axi_interconnect_0_M01_AXI_RREADY     )\
(-label "RRESP  " ${S_PATH}/axi_interconnect_0_M01_AXI_RRESP      )\
(-label "RVALID " ${S_PATH}/axi_interconnect_0_M01_AXI_RVALID     ))\
(-vgroup GPIO \
(-label "GPIO   " ${S_PATH}/axi_gpio_1_GPIO_TRI_O                 ))\
(-vgroup STATUS \
(-label "Transaction status" -alias slicing_status /axi_monitor/slave0.virtual_status.stat ))
