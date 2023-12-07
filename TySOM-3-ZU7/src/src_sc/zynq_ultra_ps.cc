/*
 * Top level for Zynq Cosim
 *
 * Copyright (c) 2020 REDS
 * Written by Rick Wertenbroek
 * Adjusted for Aldec Riviera-PRO - R&B 2021 Aldec
 * Adjusted for ZynqMP - Zbyszek Tuchewicz 2022 Aldec
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "zynq_ultra_ps.h"

void zynqmp_ps::before_end_of_elaboration () {
    // QEMU configuration
    ///////////////////////
    sc_get_param("QEMU_PATH_TO_SOCKET_G", QEMU_PATH_TO_SOCKET_G);
    sc_get_param("QEMU_SYNC_QUANTUM_G",QEMU_SYNC_QUANTUM_G);

    cout << "Debug: QEMU connection path is : " << QEMU_PATH_TO_SOCKET_G << endl;

    // Clocks
    ///////////
    // Internal clocks periods, negative value means unused (tied to 0)
    // Better to use slow clocks for simulation (otherwise simulation will be very slow)
    sc_get_param("FCLK_CLK0_PERIOD_IN_NS_G",FCLK_CLK0_PERIOD_IN_NS_G);
    sc_get_param("FCLK_CLK1_PERIOD_IN_NS_G",FCLK_CLK1_PERIOD_IN_NS_G);
    sc_get_param("FCLK_CLK2_PERIOD_IN_NS_G",FCLK_CLK2_PERIOD_IN_NS_G);
    sc_get_param("FCLK_CLK3_PERIOD_IN_NS_G",FCLK_CLK3_PERIOD_IN_NS_G);

    // AXI GP Masters
    ///////////////////
    sc_get_param("M_AXI_HPM0_FPD_ENABLE_G",M_AXI_HPM0_FPD_ENABLE_G);
    sc_get_param("M_AXI_HPM1_FPD_ENABLE_G",M_AXI_HPM1_FPD_ENABLE_G);
    sc_get_param("M_AXI_HPM0_LPD_ENABLE_G",M_AXI_HPM0_LPD_ENABLE_G);


    sc_get_param("S_AXI_HPC0_FPD_ENABLE_G",S_AXI_HPC0_FPD_ENABLE_G);
    sc_get_param("S_AXI_HPC1_FPD_ENABLE_G",S_AXI_HPC1_FPD_ENABLE_G);
    sc_get_param("S_AXI_HP0_FPD_ENABLE_G",S_AXI_HP0_FPD_ENABLE_G);
    sc_get_param("S_AXI_HP1_FPD_ENABLE_G",S_AXI_HP1_FPD_ENABLE_G);
    sc_get_param("S_AXI_HP2_FPD_ENABLE_G",S_AXI_HP2_FPD_ENABLE_G);
    sc_get_param("S_AXI_HP3_FPD_ENABLE_G",S_AXI_HP3_FPD_ENABLE_G);

    sc_get_param("S_AXI_LPD_ENABLE_G",S_AXI_LPD_ENABLE_G);
    sc_get_param("S_AXI_ACP_FPD_ENABLE_G",S_AXI_ACP_FPD_ENABLE_G);
    sc_get_param("S_AXI_ACE_FPD_ENABLE_G",S_AXI_ACE_FPD_ENABLE_G);

        // FCLK_CLK1
        if (FCLK_CLK1_PERIOD_IN_NS_G > 0) {
            fclk_clk1_gen = new sc_clock("fclk_clk1_gen", sc_time(FCLK_CLK1_PERIOD_IN_NS_G, SC_NS));
            SC_METHOD(fclk_clk1_assign);
            sensitive << fclk_clk1_gen->signal();
            dont_initialize();
        } else {
            //SC_THREAD(fclk_clk1_disable);
            fclk_clk1_disable();
        }
        // FCLK_CLK2
        if (FCLK_CLK2_PERIOD_IN_NS_G > 0) {
            fclk_clk2_gen = new sc_clock("fclk_clk2_gen", sc_time(FCLK_CLK2_PERIOD_IN_NS_G, SC_NS));
            SC_METHOD(fclk_clk2_assign);
            sensitive << fclk_clk2_gen->signal();
            dont_initialize();
        } else {
            //SC_THREAD(fclk_clk2_disable);
            fclk_clk2_disable();
        }
        // FCLK_CLK3
        if (FCLK_CLK3_PERIOD_IN_NS_G > 0) {
            fclk_clk3_gen = new sc_clock("fclk_clk3_gen", sc_time(FCLK_CLK3_PERIOD_IN_NS_G, SC_NS));
            SC_METHOD(fclk_clk3_assign);
            sensitive << fclk_clk3_gen->signal();
            dont_initialize();
        } else {
            //SC_THREAD(fclk_clk3_disable);
            fclk_clk3_disable();
        }
/*
int ADDR_WIDTH,
    int DATA_WIDTH,
    int ID_WIDTH = 8,
    int AxLEN_WIDTH = 8,
    int AxLOCK_WIDTH = 1,
    int AWUSER_WIDTH = 2,
    int ARUSER_WIDTH = 2,
    int WUSER_WIDTH = 2,
    int RUSER_WIDTH = 2,
    int BUSER_WIDTH = 2,
    int ACE_MODE = ACE_MODE_OFF,
    int CD_DATA_WIDTH = DATA_WIDTH>
*/
  if (M_AXI_HPM0_FPD_ENABLE_G > 0) {
            tlm2axi_gp0 = new tlm2axi_bridge<M_AXI_HPM0_FPD_ADDR_WIDTH_G,
                                             M_AXI_HPM0_FPD_DATA_WIDTH_G,
                                             M_AXI_HPM0_FPD_ID_WIDTH_G,
                                             M_AXI_HPM0_FPD_AXLEN_WIDTH_G,
                                             M_AXI_HPM0_FPD_AXLOCK_WIDTH_G,
                                             0,
                                             0,
                                             0,
                                             0,
                                             0>("tlm2axi_gp0");

            zynq.s_axi_hpm_fpd[0]->bind(tlm2axi_gp0->tgt_socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                      */ tlm2axi_gp0->clk(     maxigp0_aclk    );
            /* sc_in<bool>                      */ tlm2axi_gp0->resetn(  maxigp0_aresetn );

            /* Write address channel.  */
            /* sc_out<bool >                    */ tlm2axi_gp0->awvalid( maxigp0_awvalid );
            /* sc_in<bool >                     */ tlm2axi_gp0->awready( maxigp0_awready );
            /* sc_out<sc_bv<ADDR_WIDTH> >       */ tlm2axi_gp0->awaddr(  maxigp0_awaddr  );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp0->awprot(  maxigp0_awprot  );
            /* sc_out<AXISignal(AWUSER_WIDTH) > */ //tlm2axi_gp0->awuser();
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp0->awregion(maxigp0_awregion);
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp0->awqos(   maxigp0_awqos   );
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp0->awcache( maxigp0_awcache );
            /* sc_out<sc_bv<2> >                */ tlm2axi_gp0->awburst( maxigp0_awburst );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp0->awsize(  maxigp0_awsize  );
            /* sc_out<AXISignal(AxLEN_WIDTH) >  */ tlm2axi_gp0->awlen(   maxigp0_awlen   );
            /* sc_out<AXISignal(ID_WIDTH) >     */ tlm2axi_gp0->awid(    maxigp0_awid    );
            /* sc_out<AXISignal(AxLOCK_WIDTH) > */ tlm2axi_gp0->awlock(  maxigp0_awlock  );

            /* Write data channel.  */
            /* sc_out<AXISignal(ID_WIDTH) >    */  //tlm2axi_gp0->wid(     maxigp0_wid    );
            /* sc_out<bool >                   */  tlm2axi_gp0->wvalid(  maxigp0_wvalid );
            /* sc_in<bool >                    */  tlm2axi_gp0->wready(  maxigp0_wready );
            /* sc_out<sc_bv<DATA_WIDTH> >      */  tlm2axi_gp0->wdata(   maxigp0_wdata  );
            /* sc_out<sc_bv<DATA_WIDTH/8> >    */  tlm2axi_gp0->wstrb(   maxigp0_wstrb  );
            /* sc_out<AXISignal(WUSER_WIDTH) > */  //tlm2axi_gp0->wuser();
            /* sc_out<bool >                   */  tlm2axi_gp0->wlast(   maxigp0_wlast  );

            /* Write response channel.  */
            /* sc_in<bool >                   */   tlm2axi_gp0->bvalid(  maxigp0_bvalid );
            /* sc_out<bool >                  */   tlm2axi_gp0->bready(  maxigp0_bready );
            /* sc_in<sc_bv<2> >               */   tlm2axi_gp0->bresp(   maxigp0_bresp  );
            /* sc_in<AXISignal(BUSER_WIDTH) > */   //tlm2axi_gp0->buser();
            /* sc_in<AXISignal(ID_WIDTH) >    */   tlm2axi_gp0->bid(     maxigp0_bid    );

            /* Read address channel.  */
            /* sc_out<bool >                    */ tlm2axi_gp0->arvalid( maxigp0_arvalid );
            /* sc_in<bool >                     */ tlm2axi_gp0->arready( maxigp0_arready );
            /* sc_out<sc_bv<ADDR_WIDTH> >       */ tlm2axi_gp0->araddr ( maxigp0_araddr  );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp0->arprot ( maxigp0_arprot  );
            /* sc_out<AXISignal(ARUSER_WIDTH) > */ //tlm2axi_gp0->aruser();
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp0->arregion(maxigp0_arregion);
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp0->arqos  ( maxigp0_arqos   );
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp0->arcache( maxigp0_arcache );
            /* sc_out<sc_bv<2> >                */ tlm2axi_gp0->arburst( maxigp0_arburst );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp0->arsize(  maxigp0_arsize  );
            /* sc_out<AXISignal(AxLEN_WIDTH) >  */ tlm2axi_gp0->arlen(   maxigp0_arlen   );
            /* sc_out<AXISignal(ID_WIDTH) >     */ tlm2axi_gp0->arid(    maxigp0_arid    );
            /* sc_out<AXISignal(AxLOCK_WIDTH) > */ tlm2axi_gp0->arlock(  maxigp0_arlock  );

            /* Read data channel.  */
            /* sc_in<bool >                   */   tlm2axi_gp0->rvalid(  maxigp0_rvalid );
            /* sc_out<bool >                  */   tlm2axi_gp0->rready(  maxigp0_rready );
            /* sc_in<sc_bv<DATA_WIDTH> >      */   tlm2axi_gp0->rdata(   maxigp0_rdata  );
            /* sc_in<sc_bv<2> >               */   tlm2axi_gp0->rresp(   maxigp0_rresp  );
            /* sc_in<AXISignal(RUSER_WIDTH) > */   //tlm2axi_gp0->ruser();
            /* sc_in<AXISignal(ID_WIDTH) >    */   tlm2axi_gp0->rid(     maxigp0_rid    );
            /* sc_in<bool >                   */   tlm2axi_gp0->rlast(   maxigp0_rlast  );

        }

        // M AXI GP1
        //////////////
        if (M_AXI_HPM1_FPD_ENABLE_G > 0) {
            tlm2axi_gp1 = new tlm2axi_bridge<M_AXI_HPM1_FPD_ADDR_WIDTH_G,
                                             M_AXI_HPM1_FPD_DATA_WIDTH_G,
                                             M_AXI_HPM1_FPD_ID_WIDTH_G,
                                             M_AXI_HPM1_FPD_AXLEN_WIDTH_G,
                                             M_AXI_HPM1_FPD_AXLOCK_WIDTH_G,
                                             0,
                                             0,
                                             0,
                                             0,
                                             0>("tlm2axi_gp1");

            zynq.s_axi_hpm_fpd[1]->bind(tlm2axi_gp1->tgt_socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                      */ tlm2axi_gp1->clk(     maxigp1_aclk    );
            /* sc_in<bool>                      */ tlm2axi_gp1->resetn(  maxigp1_aresetn );

            /* Write address channel.  */
            /* sc_out<bool >                    */ tlm2axi_gp1->awvalid( maxigp1_awvalid );
            /* sc_in<bool >                     */ tlm2axi_gp1->awready( maxigp1_awready );
            /* sc_out<sc_bv<ADDR_WIDTH> >       */ tlm2axi_gp1->awaddr(  maxigp1_awaddr  );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp1->awprot(  maxigp1_awprot  );
            /* sc_out<AXISignal(AWUSER_WIDTH) > */ //tlm2axi_gp1->awuser();
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp1->awregion(maxigp1_awregion);
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp1->awqos(   maxigp1_awqos   );
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp1->awcache( maxigp1_awcache );
            /* sc_out<sc_bv<2> >                */ tlm2axi_gp1->awburst( maxigp1_awburst );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp1->awsize(  maxigp1_awsize  );
            /* sc_out<AXISignal(AxLEN_WIDTH) >  */ tlm2axi_gp1->awlen(   maxigp1_awlen   );
            /* sc_out<AXISignal(ID_WIDTH) >     */ tlm2axi_gp1->awid(    maxigp1_awid    );
            /* sc_out<AXISignal(AxLOCK_WIDTH) > */ tlm2axi_gp1->awlock(  maxigp1_awlock  );

            /* Write data channel.  */
            /* sc_out<AXISignal(ID_WIDTH) >    */  //tlm2axi_gp1->wid(     maxigp1_wid    );
            /* sc_out<bool >                   */  tlm2axi_gp1->wvalid(  maxigp1_wvalid );
            /* sc_in<bool >                    */  tlm2axi_gp1->wready(  maxigp1_wready );
            /* sc_out<sc_bv<DATA_WIDTH> >      */  tlm2axi_gp1->wdata(   maxigp1_wdata  );
            /* sc_out<sc_bv<DATA_WIDTH/8> >    */  tlm2axi_gp1->wstrb(   maxigp1_wstrb  );
            /* sc_out<AXISignal(WUSER_WIDTH) > */  //tlm2axi_gp1->wuser();
            /* sc_out<bool >                   */  tlm2axi_gp1->wlast(   maxigp1_wlast  );

            /* Write response channel.  */
            /* sc_in<bool >                   */   tlm2axi_gp1->bvalid(  maxigp1_bvalid );
            /* sc_out<bool >                  */   tlm2axi_gp1->bready(  maxigp1_bready );
            /* sc_in<sc_bv<2> >               */   tlm2axi_gp1->bresp(   maxigp1_bresp  );
            /* sc_in<AXISignal(BUSER_WIDTH) > */   //tlm2axi_gp1->buser();
            /* sc_in<AXISignal(ID_WIDTH) >    */   tlm2axi_gp1->bid(     maxigp1_bid    );

            /* Read address channel.  */
            /* sc_out<bool >                    */ tlm2axi_gp1->arvalid( maxigp1_arvalid );
            /* sc_in<bool >                     */ tlm2axi_gp1->arready( maxigp1_arready );
            /* sc_out<sc_bv<ADDR_WIDTH> >       */ tlm2axi_gp1->araddr ( maxigp1_araddr  );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp1->arprot ( maxigp1_arprot  );
            /* sc_out<AXISignal(ARUSER_WIDTH) > */ //tlm2axi_gp1->aruser();
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp1->arregion(maxigp1_arregion);
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp1->arqos  ( maxigp1_arqos   );
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp1->arcache( maxigp1_arcache );
            /* sc_out<sc_bv<2> >                */ tlm2axi_gp1->arburst( maxigp1_arburst );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp1->arsize(  maxigp1_arsize  );
            /* sc_out<AXISignal(AxLEN_WIDTH) >  */ tlm2axi_gp1->arlen(   maxigp1_arlen   );
            /* sc_out<AXISignal(ID_WIDTH) >     */ tlm2axi_gp1->arid(    maxigp1_arid    );
            /* sc_out<AXISignal(AxLOCK_WIDTH) > */ tlm2axi_gp1->arlock(  maxigp1_arlock  );

            /* Read data channel.  */
            /* sc_in<bool >                   */   tlm2axi_gp1->rvalid(  maxigp1_rvalid );
            /* sc_out<bool >                  */   tlm2axi_gp1->rready(  maxigp1_rready );
            /* sc_in<sc_bv<DATA_WIDTH> >      */   tlm2axi_gp1->rdata(   maxigp1_rdata  );
            /* sc_in<sc_bv<2> >               */   tlm2axi_gp1->rresp(   maxigp1_rresp  );
            /* sc_in<AXISignal(RUSER_WIDTH) > */   //tlm2axi_gp1->ruser();
            /* sc_in<AXISignal(ID_WIDTH) >    */   tlm2axi_gp1->rid(     maxigp1_rid    );
            /* sc_in<bool >                   */   tlm2axi_gp1->rlast(   maxigp1_rlast  );
        }


        // M AXI GP2
        //////////////
        if (M_AXI_HPM0_LPD_ENABLE_G > 0) {
            tlm2axi_gp2 = new tlm2axi_bridge<M_AXI_HPM0_LPD_ADDR_WIDTH_G,
                                             M_AXI_HPM0_LPD_DATA_WIDTH_G,
                                             M_AXI_HPM0_LPD_ID_WIDTH_G,
                                             M_AXI_HPM0_LPD_AXLEN_WIDTH_G,
                                             M_AXI_HPM0_LPD_AXLOCK_WIDTH_G,
                                             0,
                                             0,
                                             0,
                                             0,
                                             0>("tlm2axi_gp2");

            zynq.s_axi_hpm_lpd->bind(tlm2axi_gp2->tgt_socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                      */ tlm2axi_gp2->clk(     maxigp2_aclk    );
            /* sc_in<bool>                      */ tlm2axi_gp2->resetn(  maxigp2_aresetn );

            /* Write address channel.  */
            /* sc_out<bool >                    */ tlm2axi_gp2->awvalid( maxigp2_awvalid );
            /* sc_in<bool >                     */ tlm2axi_gp2->awready( maxigp2_awready );
            /* sc_out<sc_bv<ADDR_WIDTH> >       */ tlm2axi_gp2->awaddr(  maxigp2_awaddr  );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp2->awprot(  maxigp2_awprot  );
            /* sc_out<AXISignal(AWUSER_WIDTH) > */ //tlm2axi_gp2->awuser();
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp2->awregion(maxigp2_awregion);
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp2->awqos(   maxigp2_awqos   );
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp2->awcache( maxigp2_awcache );
            /* sc_out<sc_bv<2> >                */ tlm2axi_gp2->awburst( maxigp2_awburst );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp2->awsize(  maxigp2_awsize  );
            /* sc_out<AXISignal(AxLEN_WIDTH) >  */ tlm2axi_gp2->awlen(   maxigp2_awlen   );
            /* sc_out<AXISignal(ID_WIDTH) >     */ tlm2axi_gp2->awid(    maxigp2_awid    );
            /* sc_out<AXISignal(AxLOCK_WIDTH) > */ tlm2axi_gp2->awlock(  maxigp2_awlock  );

            /* Write data channel.  */
            /* sc_out<AXISignal(ID_WIDTH) >    */  //tlm2axi_gp2->wid(     maxigp2_wid    );
            /* sc_out<bool >                   */  tlm2axi_gp2->wvalid(  maxigp2_wvalid );
            /* sc_in<bool >                    */  tlm2axi_gp2->wready(  maxigp2_wready );
            /* sc_out<sc_bv<DATA_WIDTH> >      */  tlm2axi_gp2->wdata(   maxigp2_wdata  );
            /* sc_out<sc_bv<DATA_WIDTH/8> >    */  tlm2axi_gp2->wstrb(   maxigp2_wstrb  );
            /* sc_out<AXISignal(WUSER_WIDTH) > */  //tlm2axi_gp2->wuser();
            /* sc_out<bool >                   */  tlm2axi_gp2->wlast(   maxigp2_wlast  );

            /* Write response channel.  */
            /* sc_in<bool >                   */   tlm2axi_gp2->bvalid(  maxigp2_bvalid );
            /* sc_out<bool >                  */   tlm2axi_gp2->bready(  maxigp2_bready );
            /* sc_in<sc_bv<2> >               */   tlm2axi_gp2->bresp(   maxigp2_bresp  );
            /* sc_in<AXISignal(BUSER_WIDTH) > */   //tlm2axi_gp2->buser();
            /* sc_in<AXISignal(ID_WIDTH) >    */   tlm2axi_gp2->bid(     maxigp2_bid    );

            /* Read address channel.  */
            /* sc_out<bool >                    */ tlm2axi_gp2->arvalid( maxigp2_arvalid );
            /* sc_in<bool >                     */ tlm2axi_gp2->arready( maxigp2_arready );
            /* sc_out<sc_bv<ADDR_WIDTH> >       */ tlm2axi_gp2->araddr ( maxigp2_araddr  );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp2->arprot ( maxigp2_arprot  );
            /* sc_out<AXISignal(ARUSER_WIDTH) > */ //tlm2axi_gp2->aruser();
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp2->arregion(maxigp2_arregion);
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp2->arqos  ( maxigp2_arqos   );
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp2->arcache( maxigp2_arcache );
            /* sc_out<sc_bv<2> >                */ tlm2axi_gp2->arburst( maxigp2_arburst );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp2->arsize(  maxigp2_arsize  );
            /* sc_out<AXISignal(AxLEN_WIDTH) >  */ tlm2axi_gp2->arlen(   maxigp2_arlen   );
            /* sc_out<AXISignal(ID_WIDTH) >     */ tlm2axi_gp2->arid(    maxigp2_arid    );
            /* sc_out<AXISignal(AxLOCK_WIDTH) > */ tlm2axi_gp2->arlock(  maxigp2_arlock  );

            /* Read data channel.  */
            /* sc_in<bool >                   */   tlm2axi_gp2->rvalid(  maxigp2_rvalid );
            /* sc_out<bool >                  */   tlm2axi_gp2->rready(  maxigp2_rready );
            /* sc_in<sc_bv<DATA_WIDTH> >      */   tlm2axi_gp2->rdata(   maxigp2_rdata  );
            /* sc_in<sc_bv<2> >               */   tlm2axi_gp2->rresp(   maxigp2_rresp  );
            /* sc_in<AXISignal(RUSER_WIDTH) > */   //tlm2axi_gp2->ruser();
            /* sc_in<AXISignal(ID_WIDTH) >    */   tlm2axi_gp2->rid(     maxigp2_rid    );
            /* sc_in<bool >                   */   tlm2axi_gp2->rlast(   maxigp2_rlast  );
        }


        // S AXI GP0
        //////////////
        if (S_AXI_HP1_FPD_ENABLE_G > 0) {
            axi2tlm_gp0 = new axi2tlm_bridge<S_AXI_HPC0_FPD_ADDR_WIDTH_G,
                                             S_AXI_HPC0_FPD_DATA_WIDTH_G,
                                             S_AXI_HPC0_FPD_ID_WIDTH_G,
                                             S_AXI_HPC0_FPD_AXLEN_WIDTH_G,
                                             S_AXI_HPC0_FPD_AXLOCK_WIDTH_G,
                                             0,
                                             0,
                                             0,
                                             0,
                                             0> ("axi2tlm_gp0");

            zynq.s_axi_hpc_fpd[0]->bind(axi2tlm_gp0->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_gp0->clk(     saxigp0_aclk    );
            /* sc_in<bool>                     */ axi2tlm_gp0->resetn(  saxigp0_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp0->awvalid( saxigp0_awvalid );
            /* sc_out<bool>                    */ axi2tlm_gp0->awready( saxigp0_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp0->awaddr(  saxigp0_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp0->awprot(  saxigp0_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_gp0->awuser(  saxigp0_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp0->awregion(saxigp0_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp0->awqos(   saxigp0_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp0->awcache( saxigp0_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp0->awburst( saxigp0_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp0->awsize(  saxigp0_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp0->awlen(   saxigp0_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp0->awid(    saxigp0_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp0->awlock(  saxigp0_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_gp0->wid(     saxigp0_wid     );
            /* sc_in<bool>                     */ axi2tlm_gp0->wvalid(  saxigp0_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_gp0->wready(  saxigp0_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_gp0->wdata(   saxigp0_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_gp0->wstrb(   saxigp0_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_gp0->wuser(   saxigp0_wuser   );
            /* sc_in<bool>                     */ axi2tlm_gp0->wlast(   saxigp0_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp0->bvalid(  saxigp0_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp0->bready(  saxigp0_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp0->bresp(   saxigp0_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_gp0->buser(   saxigp0_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp0->bid(     saxigp0_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp0->arvalid( saxigp0_arvalid );
            /* sc_out<bool>                    */ axi2tlm_gp0->arready( saxigp0_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp0->araddr(  saxigp0_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp0->arprot(  saxigp0_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_gp0->aruser(  saxigp0_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp0->arregion(saxigp0_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp0->arqos(   saxigp0_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp0->arcache( saxigp0_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp0->arburst( saxigp0_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp0->arsize(  saxigp0_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp0->arlen(   saxigp0_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp0->arid(    saxigp0_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp0->arlock(  saxigp0_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp0->rvalid(  saxigp0_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp0->rready(  saxigp0_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_gp0->rdata(   saxigp0_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp0->rresp(   saxigp0_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_gp0->ruser(   saxigp0_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp0->rid(     saxigp0_rid     );
            /* sc_out<bool>                    */ axi2tlm_gp0->rlast(   saxigp0_rlast   );
        }


        // S AXI GP1
        //////////////
        if (S_AXI_HPC1_FPD_ENABLE_G > 0) {
            axi2tlm_gp1 = new axi2tlm_bridge<S_AXI_HPC1_FPD_ADDR_WIDTH_G,
                                             S_AXI_HPC1_FPD_DATA_WIDTH_G,
                                             S_AXI_HPC1_FPD_ID_WIDTH_G,
                                             S_AXI_HPC1_FPD_AXLEN_WIDTH_G,
                                             S_AXI_HPC1_FPD_AXLOCK_WIDTH_G,
                                             0,
                                             0,
                                             0,
                                             0,
                                             0> ("axi2tlm_gp1");

            zynq.s_axi_hpc_fpd[1]->bind(axi2tlm_gp1->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_gp1->clk(     saxigp1_aclk    );
            /* sc_in<bool>                     */ axi2tlm_gp1->resetn(  saxigp1_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp1->awvalid( saxigp1_awvalid );
            /* sc_out<bool>                    */ axi2tlm_gp1->awready( saxigp1_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp1->awaddr(  saxigp1_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp1->awprot(  saxigp1_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_gp1->awuser(  saxigp1_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp1->awregion(saxigp1_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp1->awqos(   saxigp1_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp1->awcache( saxigp1_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp1->awburst( saxigp1_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp1->awsize(  saxigp1_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp1->awlen(   saxigp1_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp1->awid(    saxigp1_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp1->awlock(  saxigp1_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_gp1->wid(     saxigp1_wid     );
            /* sc_in<bool>                     */ axi2tlm_gp1->wvalid(  saxigp1_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_gp1->wready(  saxigp1_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_gp1->wdata(   saxigp1_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_gp1->wstrb(   saxigp1_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_gp1->wuser(   saxigp1_wuser   );
            /* sc_in<bool>                     */ axi2tlm_gp1->wlast(   saxigp1_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp1->bvalid(  saxigp1_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp1->bready(  saxigp1_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp1->bresp(   saxigp1_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_gp1->buser(   saxigp1_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp1->bid(     saxigp1_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp1->arvalid( saxigp1_arvalid );
            /* sc_out<bool>                    */ axi2tlm_gp1->arready( saxigp1_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp1->araddr(  saxigp1_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp1->arprot(  saxigp1_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_gp1->aruser(  saxigp1_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp1->arregion(saxigp1_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp1->arqos(   saxigp1_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp1->arcache( saxigp1_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp1->arburst( saxigp1_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp1->arsize(  saxigp1_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp1->arlen(   saxigp1_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp1->arid(    saxigp1_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp1->arlock(  saxigp1_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp1->rvalid(  saxigp1_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp1->rready(  saxigp1_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_gp1->rdata(   saxigp1_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp1->rresp(   saxigp1_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_gp1->ruser(   saxigp1_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp1->rid(     saxigp1_rid     );
            /* sc_out<bool>                    */ axi2tlm_gp1->rlast(   saxigp1_rlast   );
        }

        // S AXI HP0
        //////////////
        if (S_AXI_HP0_FPD_ENABLE_G > 0) {
            axi2tlm_gp2 = new axi2tlm_bridge<S_AXI_HP0_FPD_ADDR_WIDTH_G,
                                             S_AXI_HP0_FPD_DATA_WIDTH_G,
                                             S_AXI_HP0_FPD_ID_WIDTH_G,
                                             S_AXI_HP0_FPD_AXLEN_WIDTH_G,
                                             S_AXI_HP0_FPD_AXLOCK_WIDTH_G,
                                             0,
                                             0,
                                             0,
                                             0,
                                             0> ("axi2tlm_gp2");

            zynq.s_axi_hp_fpd[0]->bind(axi2tlm_gp2->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_gp2->clk(     saxigp2_aclk    );
            /* sc_in<bool>                     */ axi2tlm_gp2->resetn(  saxigp2_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp2->awvalid( saxigp2_awvalid );
            /* sc_out<bool>                    */ axi2tlm_gp2->awready( saxigp2_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp2->awaddr(  saxigp2_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp2->awprot(  saxigp2_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_gp2->awuser(  saxigp2_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp2->awregion(saxigp2_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp2->awqos(   saxigp2_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp2->awcache( saxigp2_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp2->awburst( saxigp2_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp2->awsize(  saxigp2_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp2->awlen(   saxigp2_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp2->awid(    saxigp2_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp2->awlock(  saxigp2_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_gp2->wid(     saxigp2_wid     );
            /* sc_in<bool>                     */ axi2tlm_gp2->wvalid(  saxigp2_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_gp2->wready(  saxigp2_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_gp2->wdata(   saxigp2_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_gp2->wstrb(   saxigp2_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_gp2->wuser(   saxigp2_wuser   );
            /* sc_in<bool>                     */ axi2tlm_gp2->wlast(   saxigp2_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp2->bvalid(  saxigp2_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp2->bready(  saxigp2_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp2->bresp(   saxigp2_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_gp2->buser(   saxigp2_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp2->bid(     saxigp2_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp2->arvalid( saxigp2_arvalid );
            /* sc_out<bool>                    */ axi2tlm_gp2->arready( saxigp2_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp2->araddr(  saxigp2_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp2->arprot(  saxigp2_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_gp2->aruser(  saxigp2_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp2->arregion(saxigp2_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp2->arqos(   saxigp2_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp2->arcache( saxigp2_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp2->arburst( saxigp2_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp2->arsize(  saxigp2_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp2->arlen(   saxigp2_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp2->arid(    saxigp2_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp2->arlock(  saxigp2_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp2->rvalid(  saxigp2_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp2->rready(  saxigp2_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_gp2->rdata(   saxigp2_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp2->rresp(   saxigp2_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_gp2->ruser(   saxigp2_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp2->rid(     saxigp2_rid     );
            /* sc_out<bool>                    */ axi2tlm_gp2->rlast(   saxigp2_rlast   );
        }

        // S AXI HP1
        //////////////
        if (S_AXI_HP1_FPD_ENABLE_G > 0) {
            axi2tlm_gp3 = new axi2tlm_bridge<S_AXI_HP1_FPD_ADDR_WIDTH_G,
                                             S_AXI_HP1_FPD_DATA_WIDTH_G,
                                             S_AXI_HP1_FPD_ID_WIDTH_G,
                                             S_AXI_HP1_FPD_AXLEN_WIDTH_G,
                                             S_AXI_HP1_FPD_AXLOCK_WIDTH_G,
                                             0,
                                             0,
                                             0,
                                             0,
                                             0> ("axi2tlm_gp3");

            zynq.s_axi_hp_fpd[1]->bind(axi2tlm_gp3->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_gp3->clk(     saxigp3_aclk    );
            /* sc_in<bool>                     */ axi2tlm_gp3->resetn(  saxigp3_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp3->awvalid( saxigp3_awvalid );
            /* sc_out<bool>                    */ axi2tlm_gp3->awready( saxigp3_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp3->awaddr(  saxigp3_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp3->awprot(  saxigp3_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_gp3->awuser(  saxigp3_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp3->awregion(saxigp3_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp3->awqos(   saxigp3_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp3->awcache( saxigp3_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp3->awburst( saxigp3_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp3->awsize(  saxigp3_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp3->awlen(   saxigp3_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp3->awid(    saxigp3_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp3->awlock(  saxigp3_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_gp3->wid(     saxigp3_wid     );
            /* sc_in<bool>                     */ axi2tlm_gp3->wvalid(  saxigp3_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_gp3->wready(  saxigp3_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_gp3->wdata(   saxigp3_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_gp3->wstrb(   saxigp3_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_gp3->wuser(   saxigp3_wuser   );
            /* sc_in<bool>                     */ axi2tlm_gp3->wlast(   saxigp3_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp3->bvalid(  saxigp3_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp3->bready(  saxigp3_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp3->bresp(   saxigp3_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_gp3->buser(   saxigp3_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp3->bid(     saxigp3_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp3->arvalid( saxigp3_arvalid );
            /* sc_out<bool>                    */ axi2tlm_gp3->arready( saxigp3_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp3->araddr(  saxigp3_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp3->arprot(  saxigp3_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_gp3->aruser(  saxigp3_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp3->arregion(saxigp3_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp3->arqos(   saxigp3_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp3->arcache( saxigp3_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp3->arburst( saxigp3_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp3->arsize(  saxigp3_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp3->arlen(   saxigp3_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp3->arid(    saxigp3_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp3->arlock(  saxigp3_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp3->rvalid(  saxigp3_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp3->rready(  saxigp3_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_gp3->rdata(   saxigp3_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp3->rresp(   saxigp3_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_gp3->ruser(   saxigp3_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp3->rid(     saxigp3_rid     );
            /* sc_out<bool>                    */ axi2tlm_gp3->rlast(   saxigp3_rlast   );
        }

        // S AXI HP2
        //////////////
        if (S_AXI_HP2_FPD_ENABLE_G > 0) {
            axi2tlm_gp4 = new axi2tlm_bridge<S_AXI_HP2_FPD_ADDR_WIDTH_G,
                                             S_AXI_HP2_FPD_DATA_WIDTH_G,
                                             S_AXI_HP2_FPD_ID_WIDTH_G,
                                             S_AXI_HP2_FPD_AXLEN_WIDTH_G,
                                             S_AXI_HP2_FPD_AXLOCK_WIDTH_G,
                                             0,
                                             0,
                                             0,
                                             0,
                                             0> ("axi2tlm_gp4");

            zynq.s_axi_hp_fpd[2]->bind(axi2tlm_gp4->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_gp4->clk(     saxigp4_aclk    );
            /* sc_in<bool>                     */ axi2tlm_gp4->resetn(  saxigp4_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp4->awvalid( saxigp4_awvalid );
            /* sc_out<bool>                    */ axi2tlm_gp4->awready( saxigp4_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp4->awaddr(  saxigp4_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp4->awprot(  saxigp4_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_gp4->awuser(  saxigp4_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp4->awregion(saxigp4_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp4->awqos(   saxigp4_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp4->awcache( saxigp4_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp4->awburst( saxigp4_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp4->awsize(  saxigp4_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp4->awlen(   saxigp4_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp4->awid(    saxigp4_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp4->awlock(  saxigp4_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_gp4->wid(     saxigp4_wid     );
            /* sc_in<bool>                     */ axi2tlm_gp4->wvalid(  saxigp4_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_gp4->wready(  saxigp4_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_gp4->wdata(   saxigp4_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_gp4->wstrb(   saxigp4_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_gp4->wuser(   saxigp4_wuser   );
            /* sc_in<bool>                     */ axi2tlm_gp4->wlast(   saxigp4_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp4->bvalid(  saxigp4_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp4->bready(  saxigp4_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp4->bresp(   saxigp4_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_gp4->buser(   saxigp4_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp4->bid(     saxigp4_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp4->arvalid( saxigp4_arvalid );
            /* sc_out<bool>                    */ axi2tlm_gp4->arready( saxigp4_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp4->araddr(  saxigp4_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp4->arprot(  saxigp4_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_gp4->aruser(  saxigp4_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp4->arregion(saxigp4_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp4->arqos(   saxigp4_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp4->arcache( saxigp4_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp4->arburst( saxigp4_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp4->arsize(  saxigp4_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp4->arlen(   saxigp4_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp4->arid(    saxigp4_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp4->arlock(  saxigp4_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp4->rvalid(  saxigp4_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp4->rready(  saxigp4_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_gp4->rdata(   saxigp4_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp4->rresp(   saxigp4_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_gp4->ruser(   saxigp4_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp4->rid(     saxigp4_rid     );
            /* sc_out<bool>                    */ axi2tlm_gp4->rlast(   saxigp4_rlast   );
        }

        // S AXI HP3
        //////////////
        if (S_AXI_HP3_FPD_ENABLE_G > 0) {
            axi2tlm_gp5 = new axi2tlm_bridge<S_AXI_HP3_FPD_ADDR_WIDTH_G,
                                             S_AXI_HP3_FPD_DATA_WIDTH_G,
                                             S_AXI_HP3_FPD_ID_WIDTH_G,
                                             S_AXI_HP3_FPD_AXLEN_WIDTH_G,
                                             S_AXI_HP3_FPD_AXLOCK_WIDTH_G,
                                             0,
                                             0,
                                             0,
                                             0,
                                             0> ("axi2tlm_gp5");

            zynq.s_axi_hp_fpd[3]->bind(axi2tlm_gp5->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_gp5->clk(     saxigp5_aclk    );
            /* sc_in<bool>                     */ axi2tlm_gp5->resetn(  saxigp5_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp5->awvalid( saxigp5_awvalid );
            /* sc_out<bool>                    */ axi2tlm_gp5->awready( saxigp5_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp5->awaddr(  saxigp5_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp5->awprot(  saxigp5_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_gp5->awuser(  saxigp5_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp5->awregion(saxigp5_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp5->awqos(   saxigp5_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp5->awcache( saxigp5_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp5->awburst( saxigp5_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp5->awsize(  saxigp5_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp5->awlen(   saxigp5_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp5->awid(    saxigp5_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp5->awlock(  saxigp5_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_gp5->wid(     saxigp5_wid     );
            /* sc_in<bool>                     */ axi2tlm_gp5->wvalid(  saxigp5_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_gp5->wready(  saxigp5_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_gp5->wdata(   saxigp5_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_gp5->wstrb(   saxigp5_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_gp5->wuser(   saxigp5_wuser   );
            /* sc_in<bool>                     */ axi2tlm_gp5->wlast(   saxigp5_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp5->bvalid(  saxigp5_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp5->bready(  saxigp5_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp5->bresp(   saxigp5_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_gp5->buser(   saxigp5_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp5->bid(     saxigp5_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp5->arvalid( saxigp5_arvalid );
            /* sc_out<bool>                    */ axi2tlm_gp5->arready( saxigp5_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp5->araddr(  saxigp5_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp5->arprot(  saxigp5_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_gp5->aruser(  saxigp5_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp5->arregion(saxigp5_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp5->arqos(   saxigp5_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp5->arcache( saxigp5_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp5->arburst( saxigp5_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp5->arsize(  saxigp5_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp5->arlen(   saxigp5_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp5->arid(    saxigp5_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp5->arlock(  saxigp5_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp5->rvalid(  saxigp5_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp5->rready(  saxigp5_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_gp5->rdata(   saxigp5_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp5->rresp(   saxigp5_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_gp5->ruser(   saxigp5_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp5->rid(     saxigp5_rid     );
            /* sc_out<bool>                    */ axi2tlm_gp5->rlast(   saxigp5_rlast   );
        }





        // S AXI LPD
        //////////////
        if (S_AXI_LPD_ENABLE_G > 0) {
            axi2tlm_gp6 = new axi2tlm_bridge<S_AXI_LPD_ADDR_WIDTH_G,
                                             S_AXI_LPD_DATA_WIDTH_G,
                                             S_AXI_LPD_ID_WIDTH_G,
                                             S_AXI_LPD_AXLEN_WIDTH_G,
                                             S_AXI_LPD_AXLOCK_WIDTH_G,
                                             0,
                                             0,
                                             0,
                                             0,
                                             0> ("axi2tlm_gp6");

            zynq.s_axi_lpd->bind(axi2tlm_gp6->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_gp6->clk(     saxigp6_aclk    );
            /* sc_in<bool>                     */ axi2tlm_gp6->resetn(  saxigp6_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp6->awvalid( saxigp6_awvalid );
            /* sc_out<bool>                    */ axi2tlm_gp6->awready( saxigp6_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp6->awaddr(  saxigp6_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp6->awprot(  saxigp6_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_gp6->awuser(  saxigp6_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp6->awregion(saxigp6_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp6->awqos(   saxigp6_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp6->awcache( saxigp6_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp6->awburst( saxigp6_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp6->awsize(  saxigp6_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp6->awlen(   saxigp6_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp6->awid(    saxigp6_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp6->awlock(  saxigp6_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_gp6->wid(     saxigp6_wid     );
            /* sc_in<bool>                     */ axi2tlm_gp6->wvalid(  saxigp6_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_gp6->wready(  saxigp6_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_gp6->wdata(   saxigp6_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_gp6->wstrb(   saxigp6_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_gp6->wuser(   saxigp6_wuser   );
            /* sc_in<bool>                     */ axi2tlm_gp6->wlast(   saxigp6_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp6->bvalid(  saxigp6_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp6->bready(  saxigp6_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp6->bresp(   saxigp6_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_gp6->buser(   saxigp6_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp6->bid(     saxigp6_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp6->arvalid( saxigp6_arvalid );
            /* sc_out<bool>                    */ axi2tlm_gp6->arready( saxigp6_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp6->araddr(  saxigp6_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp6->arprot(  saxigp6_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_gp6->aruser(  saxigp6_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp6->arregion(saxigp6_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp6->arqos(   saxigp6_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp6->arcache( saxigp6_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp6->arburst( saxigp6_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp6->arsize(  saxigp6_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp6->arlen(   saxigp6_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp6->arid(    saxigp6_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp6->arlock(  saxigp6_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp6->rvalid(  saxigp6_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp6->rready(  saxigp6_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_gp6->rdata(   saxigp6_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp6->rresp(   saxigp6_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_gp6->ruser(   saxigp6_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp6->rid(     saxigp6_rid     );
            /* sc_out<bool>                    */ axi2tlm_gp6->rlast(   saxigp6_rlast   );
        }



        // S AXI ACP FPD
        //////////////
        if (S_AXI_ACP_FPD_ENABLE_G > 0) {
            axi2tlm_acp = new axi2tlm_bridge<S_AXI_ACP_FPD_ADDR_WIDTH_G,
                                             S_AXI_ACP_FPD_DATA_WIDTH_G,
                                             S_AXI_ACP_FPD_ID_WIDTH_G,
                                             S_AXI_ACP_FPD_AXLEN_WIDTH_G,
                                             S_AXI_ACP_FPD_AXLOCK_WIDTH_G,
                                             0,
                                             0,
                                             0,
                                             0,
                                             0> ("axi2tlm_acp");

            zynq.s_axi_acp_fpd->bind(axi2tlm_acp->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_acp->clk(     saxiacp_aclk    );
            /* sc_in<bool>                     */ axi2tlm_acp->resetn(  saxiacp_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_acp->awvalid( saxiacp_awvalid );
            /* sc_out<bool>                    */ axi2tlm_acp->awready( saxiacp_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_acp->awaddr(  saxiacp_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_acp->awprot(  saxiacp_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_acp->awuser(  saxiacp_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_acp->awregion(saxiacp_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_acp->awqos(   saxiacp_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_acp->awcache( saxiacp_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_acp->awburst( saxiacp_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_acp->awsize(  saxiacp_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_acp->awlen(   saxiacp_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_acp->awid(    saxiacp_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_acp->awlock(  saxiacp_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_acp->wid(     saxiacp_wid     );
            /* sc_in<bool>                     */ axi2tlm_acp->wvalid(  saxiacp_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_acp->wready(  saxiacp_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_acp->wdata(   saxiacp_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_acp->wstrb(   saxiacp_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_acp->wuser(   saxiacp_wuser   );
            /* sc_in<bool>                     */ axi2tlm_acp->wlast(   saxiacp_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_acp->bvalid(  saxiacp_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_acp->bready(  saxiacp_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_acp->bresp(   saxiacp_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_acp->buser(   saxiacp_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_acp->bid(     saxiacp_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_acp->arvalid( saxiacp_arvalid );
            /* sc_out<bool>                    */ axi2tlm_acp->arready( saxiacp_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_acp->araddr(  saxiacp_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_acp->arprot(  saxiacp_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_acp->aruser(  saxiacp_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_acp->arregion(saxiacp_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_acp->arqos(   saxiacp_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_acp->arcache( saxiacp_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_acp->arburst( saxiacp_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_acp->arsize(  saxiacp_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_acp->arlen(   saxiacp_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_acp->arid(    saxiacp_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_acp->arlock(  saxiacp_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_acp->rvalid(  saxiacp_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_acp->rready(  saxiacp_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_acp->rdata(   saxiacp_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_acp->rresp(   saxiacp_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_acp->ruser(   saxiacp_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_acp->rid(     saxiacp_rid     );
            /* sc_out<bool>                    */ axi2tlm_acp->rlast(   saxiacp_rlast   );
        }


        // S AXI ACE FPD
        //////////////
        if (S_AXI_ACE_FPD_ENABLE_G > 0) {
            axi2tlm_ace = new axi2tlm_bridge<S_AXI_ACE_FPD_ADDR_WIDTH_G,
                                             S_AXI_ACE_FPD_DATA_WIDTH_G,
                                             S_AXI_ACE_FPD_ID_WIDTH_G,
                                             S_AXI_ACE_FPD_AXLEN_WIDTH_G,
                                             S_AXI_ACE_FPD_AXLOCK_WIDTH_G,
                                             0,
                                             0,
                                             0,
                                             0,
                                             0,
                                             1> ("axi2tlm_ace");

            zynq.s_axi_ace_fpd->bind(axi2tlm_ace->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_ace->clk(     sacefpd_aclk    );
            /* sc_in<bool>                     */ axi2tlm_ace->resetn(  sacefpd_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_ace->awvalid( sacefpd_awvalid );
            /* sc_out<bool>                    */ axi2tlm_ace->awready( sacefpd_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_ace->awaddr(  sacefpd_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_ace->awprot(  sacefpd_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_ace->awuser(  sacefpd_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_ace->awregion(sacefpd_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_ace->awqos(   sacefpd_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_ace->awcache( sacefpd_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_ace->awburst( sacefpd_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_ace->awsize(  sacefpd_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_ace->awlen(   sacefpd_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_ace->awid(    sacefpd_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_ace->awlock(  sacefpd_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_ace->wid(     sacefpd_wid     );
            /* sc_in<bool>                     */ axi2tlm_ace->wvalid(  sacefpd_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_ace->wready(  sacefpd_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_ace->wdata(   sacefpd_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_ace->wstrb(   sacefpd_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_ace->wuser(   sacefpd_wuser   );
            /* sc_in<bool>                     */ axi2tlm_ace->wlast(   sacefpd_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_ace->bvalid(  sacefpd_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_ace->bready(  sacefpd_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_ace->bresp(   sacefpd_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_ace->buser(   sacefpd_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_ace->bid(     sacefpd_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_ace->arvalid( sacefpd_arvalid );
            /* sc_out<bool>                    */ axi2tlm_ace->arready( sacefpd_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_ace->araddr(  sacefpd_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_ace->arprot(  sacefpd_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_ace->aruser(  sacefpd_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_ace->arregion(sacefpd_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_ace->arqos(   sacefpd_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_ace->arcache( sacefpd_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_ace->arburst( sacefpd_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_ace->arsize(  sacefpd_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_ace->arlen(   sacefpd_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_ace->arid(    sacefpd_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_ace->arlock(  sacefpd_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_ace->rvalid(  sacefpd_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_ace->rready(  sacefpd_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_ace->rdata(   sacefpd_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_ace->rresp(   sacefpd_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_ace->ruser(   sacefpd_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_ace->rid(     sacefpd_rid     );
            /* sc_out<bool>                    */ axi2tlm_ace->rlast(   sacefpd_rlast   );


        }

zynq.tie_off();


}

SC_MODULE_EXPORT(zynqmp_ps);
