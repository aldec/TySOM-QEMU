`timescale 1ns/1ps

import axi_vip_pkg::*;

//hierarchy path to used signals
`define S_AXI_0 design_1_wrapper.design_1_i

module axi_monitor ( );
//----------------------------Declaration of channels structures-------------------------------//
	typedef struct {
		reg [31:0] AWADDR;
		reg AWVALID;
		reg AWREADY;
		reg [5:0] AWID;
	} AXI_channel_AW;
	typedef struct {
		reg [31:0] WDATA;
		reg WVALID;
		reg WREADY;
		reg WLAST;
		reg [5:0] WID;
	} AXI_channel_W;
	typedef struct {
		reg BVALID;
		reg BREADY;
		reg [1:0] BRESP;
		reg [5:0] BID;
	} AXI_channel_B;
		typedef struct {
		reg [31:0] ARADDR;
		reg ARVALID;
		reg ARREADY;
		reg [5:0] ARID;
	} AXI_channel_AR;
	typedef struct {
		reg [31:0] RDATA;
		reg RVALID;
		reg RREADY;
		reg RLAST;
		reg [1:0] RRESP;
		reg [5:0] RID;
	} AXI_channel_R;
	
	
	typedef struct {
		reg [39:0] stat;
	} virt_status;
	
	
	//main structure
	typedef struct {
		reg CLK;
		xil_axi_cmd_t cmd; //0 READ, 1 WRITE
		AXI_channel_AR AR;
		AXI_channel_R R;
		AXI_channel_AW AW;
		AXI_channel_W W;
		AXI_channel_B B;
		virt_status virtual_status;
	} axi_monitor_t;
	
	axi_monitor_t slave0;
	
//-------------------------------------clk mapping---------------------------------------------//
	always @(`S_AXI_0.zynq_ultra_ps_e_0_pl_clk0)
	begin
		slave0.CLK = `S_AXI_0.zynq_ultra_ps_e_0_pl_clk0;
	end

//--------------------------------Mapping signals on CLK's edges---------------------------------//
	
	//wywalono negedge 
	always @(posedge slave0.CLK)
	begin
		#5ps //delta cycle delay
		slave0.AR.ARADDR  = `S_AXI_0.ps8_0_axi_periph_M01_AXI_ARADDR;
		slave0.AR.ARVALID = `S_AXI_0.ps8_0_axi_periph_M01_AXI_ARVALID;
		slave0.AR.ARREADY = `S_AXI_0.ps8_0_axi_periph_M01_AXI_ARREADY;

		slave0.R.RDATA    = `S_AXI_0.ps8_0_axi_periph_M01_AXI_RDATA ;
		slave0.R.RVALID   = `S_AXI_0.ps8_0_axi_periph_M01_AXI_RVALID;
		slave0.R.RREADY   = `S_AXI_0.ps8_0_axi_periph_M01_AXI_RREADY;
		slave0.R.RRESP    = `S_AXI_0.ps8_0_axi_periph_M01_AXI_RRESP ;

		
		slave0.AW.AWADDR  = `S_AXI_0.ps8_0_axi_periph_M01_AXI_AWADDR ;
		slave0.AW.AWVALID = `S_AXI_0.ps8_0_axi_periph_M01_AXI_AWVALID;
		slave0.AW.AWREADY = `S_AXI_0.ps8_0_axi_periph_M01_AXI_AWREADY;
		
		slave0.W.WDATA    = `S_AXI_0.ps8_0_axi_periph_M01_AXI_WDATA ;
		slave0.W.WVALID   = `S_AXI_0.ps8_0_axi_periph_M01_AXI_WVALID;
		slave0.W.WREADY   = `S_AXI_0.ps8_0_axi_periph_M01_AXI_WREADY;
		
		slave0.B.BVALID   = `S_AXI_0.ps8_0_axi_periph_M01_AXI_BVALID;
		slave0.B.BREADY   = `S_AXI_0.ps8_0_axi_periph_M01_AXI_BREADY;
		slave0.B.BRESP    = `S_AXI_0.ps8_0_axi_periph_M01_AXI_BRESP ;
		
		slave0.virtual_status.stat[39:0]=status_set(slave0);
	end

//--------------------------------Setting statuses of transaction function------------------------------//

//setting status for slave0
	function bit [39:0] status_set (axi_monitor_t name);
		reg [39:0] whole_reg;
		//address write
		if (name.AW.AWVALID && name.AW.AWREADY) begin
			name.virtual_status.stat[7:0] = 8'hFF;
			$display("Simulation time: %0d ns", $time);
			whole_reg[7:0]=name.virtual_status.stat[7:0];
			$display("AWVALID value is eqal to: %0h ", whole_reg[7:0]);
		end else begin
			name.virtual_status.stat[7:0] = 8'h00;
			whole_reg[7:0]=name.virtual_status.stat[7:0];
		end
		//write data
		if (name.W.WVALID && name.W.WREADY)	begin
			name.virtual_status.stat[15:8] = 8'hFE;
			$display("Simulation time: %0d ns", $time);
			whole_reg[15:8]=name.virtual_status.stat[15:8];
			$display("WVALID value is eqal to: %0h ", whole_reg[15:8]);
		end else begin
			name.virtual_status.stat[15:8] = 8'h00;
			whole_reg[15:8]=name.virtual_status.stat[15:8];
		end
		//write response
		if (name.B.BVALID) begin
			name.virtual_status.stat[23:16] = 8'hFD;
			$display("Simulation time: %0d ns", $time);
			whole_reg[23:16]=name.virtual_status.stat[23:16];
			$display("BVALID value is eqal to: %0h ", whole_reg[23:16]);
		end else begin
			name.virtual_status.stat[23:16] = 8'h00;
			whole_reg[23:16]=name.virtual_status.stat[23:16];
		end
		//read address
		if (name.AR.ARVALID) begin
			name.virtual_status.stat[31:24] = 8'hFC;
			$display("Simulation time: %0d ns", $time);
			whole_reg[31:24]=name.virtual_status.stat[31:24];
			$display("ARVALID value is eqal to: %0h ", whole_reg[31:24]);
		end else begin
			name.virtual_status.stat[31:24] = 8'h00;
			whole_reg[31:24]=name.virtual_status.stat[31:24];
		end
		//read data
		if(name.R.RVALID && name.R.RREADY) begin
			name.virtual_status.stat[39:32] = 8'hFB;
			$display("Simulation time: %0d ns", $time);
			whole_reg[39:32]=name.virtual_status.stat[39:32];
			$display("RVALID value is eqal to: %0h ", whole_reg[39:32]);
		end else begin
			name.virtual_status.stat[39:32] = 8'h00;
			whole_reg[39:32]=name.virtual_status.stat[39:32];
		end
		
		if (whole_reg != 40'h0000000000)
			$display("Transaction status vector is equal to: %0h", whole_reg[39:0]);
			
		return whole_reg[39:0];
	endfunction
	
endmodule
