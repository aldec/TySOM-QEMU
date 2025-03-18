/*
 * Top level for Zynq Cosim
 *
 * Copyright (c) 2020 REDS
 * Written by Rick Wertenbroek
 * Adjusted for Aldec Riviera-PRO - R&B 2021 Aldec
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

#include "zynq7_ps.h"

void zynq7_ps::before_end_of_elaboration () {
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
    sc_get_param("M_AXI_GP0_ENABLE_G",M_AXI_GP0_ENABLE_G);
    sc_get_param("M_AXI_GP1_ENABLE_G",M_AXI_GP1_ENABLE_G);
    sc_get_param("S_AXI_GP0_ENABLE_G",S_AXI_GP0_ENABLE_G);
    sc_get_param("S_AXI_GP1_ENABLE_G",S_AXI_GP1_ENABLE_G);
    sc_get_param("S_AXI_HP0_ENABLE_G",S_AXI_HP0_ENABLE_G);
    sc_get_param("S_AXI_HP1_ENABLE_G",S_AXI_HP1_ENABLE_G);
    sc_get_param("S_AXI_HP2_ENABLE_G",S_AXI_HP2_ENABLE_G);
    sc_get_param("S_AXI_HP3_ENABLE_G",S_AXI_HP3_ENABLE_G);
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
  if (M_AXI_GP0_ENABLE_G > 0) {
            tlm2axi_gp0 = new tlm2axi_bridge<M_AXI_GP0_ADDR_WIDTH_G,
                                             M_AXI_GP0_DATA_WIDTH_G,
                                             M_AXI_GP0_ID_WIDTH_G,
                                             M_AXI_GP0_AXLEN_WIDTH_G,
                                             M_AXI_GP0_AXLOCK_WIDTH_G,
                                               0,
                                               0,
                                               0,
                                               0,
                                               0
                                               >("tlm2axi_gp0");



            zynq.m_axi_gp[0]->bind(tlm2axi_gp0->tgt_socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                      */ tlm2axi_gp0->clk(     m_axi_gp0_aclk    );
            /* sc_in<bool>                      */ tlm2axi_gp0->resetn(  m_axi_gp0_aresetn );

            /* Write address channel.  */
            /* sc_out<bool >                    */ tlm2axi_gp0->awvalid( m_axi_gp0_awvalid );
            /* sc_in<bool >                     */ tlm2axi_gp0->awready( m_axi_gp0_awready );
            /* sc_out<sc_bv<ADDR_WIDTH> >       */ tlm2axi_gp0->awaddr(  m_axi_gp0_awaddr  );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp0->awprot(  m_axi_gp0_awprot  );
            /* sc_out<AXISignal(AWUSER_WIDTH) > */ //tlm2axi_gp0->awuser();
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp0->awregion(m_axi_gp0_awregion);
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp0->awqos(   m_axi_gp0_awqos   );
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp0->awcache( m_axi_gp0_awcache );
            /* sc_out<sc_bv<2> >                */ tlm2axi_gp0->awburst( m_axi_gp0_awburst );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp0->awsize(  m_axi_gp0_awsize  );
            /* sc_out<AXISignal(AxLEN_WIDTH) >  */ tlm2axi_gp0->awlen(   m_axi_gp0_awlen   );
            /* sc_out<AXISignal(ID_WIDTH) >     */ tlm2axi_gp0->awid(    m_axi_gp0_awid    );
            /* sc_out<AXISignal(AxLOCK_WIDTH) > */ tlm2axi_gp0->awlock(  m_axi_gp0_awlock  );

            /* Write data channel.  */
            /* sc_out<AXISignal(ID_WIDTH) >    */  //tlm2axi_gp0->wid(     m_axi_gp0_wid    );
            /* sc_out<bool >                   */  tlm2axi_gp0->wvalid(  m_axi_gp0_wvalid );
            /* sc_in<bool >                    */  tlm2axi_gp0->wready(  m_axi_gp0_wready );
            /* sc_out<sc_bv<DATA_WIDTH> >      */  tlm2axi_gp0->wdata(   m_axi_gp0_wdata  );
            /* sc_out<sc_bv<DATA_WIDTH/8> >    */  tlm2axi_gp0->wstrb(   m_axi_gp0_wstrb  );
            /* sc_out<AXISignal(WUSER_WIDTH) > */  //tlm2axi_gp0->wuser();
            /* sc_out<bool >                   */  tlm2axi_gp0->wlast(   m_axi_gp0_wlast  );

            /* Write response channel.  */
            /* sc_in<bool >                   */   tlm2axi_gp0->bvalid(  m_axi_gp0_bvalid );
            /* sc_out<bool >                  */   tlm2axi_gp0->bready(  m_axi_gp0_bready );
            /* sc_in<sc_bv<2> >               */   tlm2axi_gp0->bresp(   m_axi_gp0_bresp  );
            /* sc_in<AXISignal(BUSER_WIDTH) > */   //tlm2axi_gp0->buser();
            /* sc_in<AXISignal(ID_WIDTH) >    */   tlm2axi_gp0->bid(     m_axi_gp0_bid    );

            /* Read address channel.  */
            /* sc_out<bool >                    */ tlm2axi_gp0->arvalid( m_axi_gp0_arvalid );
            /* sc_in<bool >                     */ tlm2axi_gp0->arready( m_axi_gp0_arready );
            /* sc_out<sc_bv<ADDR_WIDTH> >       */ tlm2axi_gp0->araddr ( m_axi_gp0_araddr  );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp0->arprot ( m_axi_gp0_arprot  );
            /* sc_out<AXISignal(ARUSER_WIDTH) > */ //tlm2axi_gp0->aruser();
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp0->arregion(m_axi_gp0_arregion);
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp0->arqos  ( m_axi_gp0_arqos   );
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp0->arcache( m_axi_gp0_arcache );
            /* sc_out<sc_bv<2> >                */ tlm2axi_gp0->arburst( m_axi_gp0_arburst );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp0->arsize(  m_axi_gp0_arsize  );
            /* sc_out<AXISignal(AxLEN_WIDTH) >  */ tlm2axi_gp0->arlen(   m_axi_gp0_arlen   );
            /* sc_out<AXISignal(ID_WIDTH) >     */ tlm2axi_gp0->arid(    m_axi_gp0_arid    );
            /* sc_out<AXISignal(AxLOCK_WIDTH) > */ tlm2axi_gp0->arlock(  m_axi_gp0_arlock  );

            /* Read data channel.  */
            /* sc_in<bool >                   */   tlm2axi_gp0->rvalid(  m_axi_gp0_rvalid );
            /* sc_out<bool >                  */   tlm2axi_gp0->rready(  m_axi_gp0_rready );
            /* sc_in<sc_bv<DATA_WIDTH> >      */   tlm2axi_gp0->rdata(   m_axi_gp0_rdata  );
            /* sc_in<sc_bv<2> >               */   tlm2axi_gp0->rresp(   m_axi_gp0_rresp  );
            /* sc_in<AXISignal(RUSER_WIDTH) > */   //tlm2axi_gp0->ruser();
            /* sc_in<AXISignal(ID_WIDTH) >    */   tlm2axi_gp0->rid(     m_axi_gp0_rid    );
            /* sc_in<bool >                   */   tlm2axi_gp0->rlast(   m_axi_gp0_rlast  );

        }

        // M AXI GP1
        //////////////
        if (M_AXI_GP1_ENABLE_G > 0) {
            tlm2axi_gp1 = new tlm2axi_bridge<M_AXI_GP1_ADDR_WIDTH_G,
                                             M_AXI_GP1_DATA_WIDTH_G,
                                             M_AXI_GP1_ID_WIDTH_G,
                                             M_AXI_GP1_AXLEN_WIDTH_G,
                                             M_AXI_GP1_AXLOCK_WIDTH_G,
                                             M_AXI_GP1_AWUSER_WIDTH_G,
                                             M_AXI_GP1_ARUSER_WIDTH_G>("tlm2axi_gp1");

            zynq.m_axi_gp[1]->bind(tlm2axi_gp1->tgt_socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                      */ tlm2axi_gp1->clk(     m_axi_gp1_aclk    );
            /* sc_in<bool>                      */ tlm2axi_gp1->resetn(  m_axi_gp1_aresetn );

            /* Write address channel.  */
            /* sc_out<bool >                    */ tlm2axi_gp1->awvalid( m_axi_gp1_awvalid );
            /* sc_in<bool >                     */ tlm2axi_gp1->awready( m_axi_gp1_awready );
            /* sc_out<sc_bv<ADDR_WIDTH> >       */ tlm2axi_gp1->awaddr(  m_axi_gp1_awaddr  );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp1->awprot(  m_axi_gp1_awprot  );
            /* sc_out<AXISignal(AWUSER_WIDTH) > */ //tlm2axi_gp1->awuser();
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp1->awregion(m_axi_gp1_awregion);
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp1->awqos(   m_axi_gp1_awqos   );
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp1->awcache( m_axi_gp1_awcache );
            /* sc_out<sc_bv<2> >                */ tlm2axi_gp1->awburst( m_axi_gp1_awburst );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp1->awsize(  m_axi_gp1_awsize  );
            /* sc_out<AXISignal(AxLEN_WIDTH) >  */ tlm2axi_gp1->awlen(   m_axi_gp1_awlen   );
            /* sc_out<AXISignal(ID_WIDTH) >     */ tlm2axi_gp1->awid(    m_axi_gp1_awid    );
            /* sc_out<AXISignal(AxLOCK_WIDTH) > */ tlm2axi_gp1->awlock(  m_axi_gp1_awlock  );

            /* Write data channel.  */
            /* sc_out<AXISignal(ID_WIDTH) >    */  //tlm2axi_gp1->wid(     m_axi_gp1_wid    );
            /* sc_out<bool >                   */  tlm2axi_gp1->wvalid(  m_axi_gp1_wvalid );
            /* sc_in<bool >                    */  tlm2axi_gp1->wready(  m_axi_gp1_wready );
            /* sc_out<sc_bv<DATA_WIDTH> >      */  tlm2axi_gp1->wdata(   m_axi_gp1_wdata  );
            /* sc_out<sc_bv<DATA_WIDTH/8> >    */  tlm2axi_gp1->wstrb(   m_axi_gp1_wstrb  );
            /* sc_out<AXISignal(WUSER_WIDTH) > */  //tlm2axi_gp1->wuser();
            /* sc_out<bool >                   */  tlm2axi_gp1->wlast(   m_axi_gp1_wlast  );

            /* Write response channel.  */
            /* sc_in<bool >                   */   tlm2axi_gp1->bvalid(  m_axi_gp1_bvalid );
            /* sc_out<bool >                  */   tlm2axi_gp1->bready(  m_axi_gp1_bready );
            /* sc_in<sc_bv<2> >               */   tlm2axi_gp1->bresp(   m_axi_gp1_bresp  );
            /* sc_in<AXISignal(BUSER_WIDTH) > */   //tlm2axi_gp1->buser();
            /* sc_in<AXISignal(ID_WIDTH) >    */   tlm2axi_gp1->bid(     m_axi_gp1_bid    );

            /* Read address channel.  */
            /* sc_out<bool >                    */ tlm2axi_gp1->arvalid( m_axi_gp1_arvalid );
            /* sc_in<bool >                     */ tlm2axi_gp1->arready( m_axi_gp1_arready );
            /* sc_out<sc_bv<ADDR_WIDTH> >       */ tlm2axi_gp1->araddr ( m_axi_gp1_araddr  );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp1->arprot ( m_axi_gp1_arprot  );
            /* sc_out<AXISignal(ARUSER_WIDTH) > */ //tlm2axi_gp1->aruser();
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp1->arregion(m_axi_gp1_arregion);
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp1->arqos  ( m_axi_gp1_arqos   );
            /* sc_out<sc_bv<4> >                */ tlm2axi_gp1->arcache( m_axi_gp1_arcache );
            /* sc_out<sc_bv<2> >                */ tlm2axi_gp1->arburst( m_axi_gp1_arburst );
            /* sc_out<sc_bv<3> >                */ tlm2axi_gp1->arsize(  m_axi_gp1_arsize  );
            /* sc_out<AXISignal(AxLEN_WIDTH) >  */ tlm2axi_gp1->arlen(   m_axi_gp1_arlen   );
            /* sc_out<AXISignal(ID_WIDTH) >     */ tlm2axi_gp1->arid(    m_axi_gp1_arid    );
            /* sc_out<AXISignal(AxLOCK_WIDTH) > */ tlm2axi_gp1->arlock(  m_axi_gp1_arlock  );

            /* Read data channel.  */
            /* sc_in<bool >                   */   tlm2axi_gp1->rvalid(  m_axi_gp1_rvalid );
            /* sc_out<bool >                  */   tlm2axi_gp1->rready(  m_axi_gp1_rready );
            /* sc_in<sc_bv<DATA_WIDTH> >      */   tlm2axi_gp1->rdata(   m_axi_gp1_rdata  );
            /* sc_in<sc_bv<2> >               */   tlm2axi_gp1->rresp(   m_axi_gp1_rresp  );
            /* sc_in<AXISignal(RUSER_WIDTH) > */   //tlm2axi_gp1->ruser();
            /* sc_in<AXISignal(ID_WIDTH) >    */   tlm2axi_gp1->rid(     m_axi_gp1_rid    );
            /* sc_in<bool >                   */   tlm2axi_gp1->rlast(   m_axi_gp1_rlast  );
        }


        // S AXI GP0
        //////////////
        if (S_AXI_GP0_ENABLE_G > 0) {
            axi2tlm_gp0 = new axi2tlm_bridge<S_AXI_GP0_ADDR_WIDTH_G,
                                             S_AXI_GP0_DATA_WIDTH_G,
                                             S_AXI_GP0_ID_WIDTH_G,
                                             S_AXI_GP0_AXLEN_WIDTH_G,
                                             S_AXI_GP0_AXLOCK_WIDTH_G,
                                             S_AXI_GP0_AWUSER_WIDTH_G,
                                             S_AXI_GP0_ARUSER_WIDTH_G> ("axi2tlm_gp0");

            zynq.s_axi_gp[0]->bind(axi2tlm_gp0->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_gp0->clk(     s_axi_gp0_aclk    );
            /* sc_in<bool>                     */ axi2tlm_gp0->resetn(  s_axi_gp0_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp0->awvalid( s_axi_gp0_awvalid );
            /* sc_out<bool>                    */ axi2tlm_gp0->awready( s_axi_gp0_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp0->awaddr(  s_axi_gp0_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp0->awprot(  s_axi_gp0_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_gp0->awuser(  s_axi_gp0_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp0->awregion(s_axi_gp0_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp0->awqos(   s_axi_gp0_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp0->awcache( s_axi_gp0_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp0->awburst( s_axi_gp0_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp0->awsize(  s_axi_gp0_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp0->awlen(   s_axi_gp0_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp0->awid(    s_axi_gp0_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp0->awlock(  s_axi_gp0_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_gp0->wid(     s_axi_gp0_wid     );
            /* sc_in<bool>                     */ axi2tlm_gp0->wvalid(  s_axi_gp0_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_gp0->wready(  s_axi_gp0_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_gp0->wdata(   s_axi_gp0_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_gp0->wstrb(   s_axi_gp0_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_gp0->wuser(   s_axi_gp0_wuser   );
            /* sc_in<bool>                     */ axi2tlm_gp0->wlast(   s_axi_gp0_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp0->bvalid(  s_axi_gp0_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp0->bready(  s_axi_gp0_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp0->bresp(   s_axi_gp0_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_gp0->buser(   s_axi_gp0_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp0->bid(     s_axi_gp0_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp0->arvalid( s_axi_gp0_arvalid );
            /* sc_out<bool>                    */ axi2tlm_gp0->arready( s_axi_gp0_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp0->araddr(  s_axi_gp0_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp0->arprot(  s_axi_gp0_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_gp0->aruser(  s_axi_gp0_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp0->arregion(s_axi_gp0_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp0->arqos(   s_axi_gp0_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp0->arcache( s_axi_gp0_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp0->arburst( s_axi_gp0_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp0->arsize(  s_axi_gp0_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp0->arlen(   s_axi_gp0_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp0->arid(    s_axi_gp0_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp0->arlock(  s_axi_gp0_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp0->rvalid(  s_axi_gp0_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp0->rready(  s_axi_gp0_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_gp0->rdata(   s_axi_gp0_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp0->rresp(   s_axi_gp0_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_gp0->ruser(   s_axi_gp0_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp0->rid(     s_axi_gp0_rid     );
            /* sc_out<bool>                    */ axi2tlm_gp0->rlast(   s_axi_gp0_rlast   );
        }


        // S AXI GP1
        //////////////
        if (S_AXI_GP1_ENABLE_G > 0) {
            axi2tlm_gp1 = new axi2tlm_bridge<S_AXI_GP1_ADDR_WIDTH_G,
                                             S_AXI_GP1_DATA_WIDTH_G,
                                             S_AXI_GP1_ID_WIDTH_G,
                                             S_AXI_GP1_AXLEN_WIDTH_G,
                                             S_AXI_GP1_AXLOCK_WIDTH_G,
                                             S_AXI_GP1_AWUSER_WIDTH_G,
                                             S_AXI_GP1_ARUSER_WIDTH_G> ("axi2tlm_gp1");

            zynq.s_axi_gp[1]->bind(axi2tlm_gp1->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_gp1->clk(     s_axi_gp1_aclk    );
            /* sc_in<bool>                     */ axi2tlm_gp1->resetn(  s_axi_gp1_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp1->awvalid( s_axi_gp1_awvalid );
            /* sc_out<bool>                    */ axi2tlm_gp1->awready( s_axi_gp1_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp1->awaddr(  s_axi_gp1_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp1->awprot(  s_axi_gp1_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_gp1->awuser(  s_axi_gp1_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp1->awregion(s_axi_gp1_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp1->awqos(   s_axi_gp1_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp1->awcache( s_axi_gp1_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp1->awburst( s_axi_gp1_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp1->awsize(  s_axi_gp1_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp1->awlen(   s_axi_gp1_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp1->awid(    s_axi_gp1_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp1->awlock(  s_axi_gp1_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_gp1->wid(     s_axi_gp1_wid     );
            /* sc_in<bool>                     */ axi2tlm_gp1->wvalid(  s_axi_gp1_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_gp1->wready(  s_axi_gp1_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_gp1->wdata(   s_axi_gp1_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_gp1->wstrb(   s_axi_gp1_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_gp1->wuser(   s_axi_gp1_wuser   );
            /* sc_in<bool>                     */ axi2tlm_gp1->wlast(   s_axi_gp1_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp1->bvalid(  s_axi_gp1_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp1->bready(  s_axi_gp1_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp1->bresp(   s_axi_gp1_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_gp1->buser(   s_axi_gp1_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp1->bid(     s_axi_gp1_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_gp1->arvalid( s_axi_gp1_arvalid );
            /* sc_out<bool>                    */ axi2tlm_gp1->arready( s_axi_gp1_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_gp1->araddr(  s_axi_gp1_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp1->arprot(  s_axi_gp1_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_gp1->aruser(  s_axi_gp1_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp1->arregion(s_axi_gp1_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp1->arqos(   s_axi_gp1_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_gp1->arcache( s_axi_gp1_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_gp1->arburst( s_axi_gp1_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_gp1->arsize(  s_axi_gp1_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_gp1->arlen(   s_axi_gp1_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_gp1->arid(    s_axi_gp1_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_gp1->arlock(  s_axi_gp1_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_gp1->rvalid(  s_axi_gp1_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_gp1->rready(  s_axi_gp1_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_gp1->rdata(   s_axi_gp1_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_gp1->rresp(   s_axi_gp1_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_gp1->ruser(   s_axi_gp1_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_gp1->rid(     s_axi_gp1_rid     );
            /* sc_out<bool>                    */ axi2tlm_gp1->rlast(   s_axi_gp1_rlast   );
        }

        // S AXI HP0
        //////////////
        if (S_AXI_HP0_ENABLE_G > 0) {
            axi2tlm_hp0 = new axi2tlm_bridge<S_AXI_HP0_ADDR_WIDTH_G,
                                             S_AXI_HP0_DATA_WIDTH_G,
                                             S_AXI_HP0_ID_WIDTH_G,
                                             S_AXI_HP0_AXLEN_WIDTH_G,
                                             S_AXI_HP0_AXLOCK_WIDTH_G,
                                             0,
                                               0,
                                               0,
                                               0,
                                               0> ("axi2tlm_hp0");

            zynq.s_axi_hp[0]->bind(axi2tlm_hp0->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_hp0->clk(     s_axi_hp0_aclk    );
            /* sc_in<bool>                     */ axi2tlm_hp0->resetn(  s_axi_hp0_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_hp0->awvalid( s_axi_hp0_awvalid );
            /* sc_out<bool>                    */ axi2tlm_hp0->awready( s_axi_hp0_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_hp0->awaddr(  s_axi_hp0_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp0->awprot(  s_axi_hp0_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_hp0->awuser(  s_axi_hp0_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp0->awregion(s_axi_hp0_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp0->awqos(   s_axi_hp0_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp0->awcache( s_axi_hp0_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_hp0->awburst( s_axi_hp0_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp0->awsize(  s_axi_hp0_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_hp0->awlen(   s_axi_hp0_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_hp0->awid(    s_axi_hp0_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_hp0->awlock(  s_axi_hp0_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_hp0->wid(     s_axi_hp0_wid     );
            /* sc_in<bool>                     */ axi2tlm_hp0->wvalid(  s_axi_hp0_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_hp0->wready(  s_axi_hp0_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_hp0->wdata(   s_axi_hp0_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_hp0->wstrb(   s_axi_hp0_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_hp0->wuser(   s_axi_hp0_wuser   );
            /* sc_in<bool>                     */ axi2tlm_hp0->wlast(   s_axi_hp0_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_hp0->bvalid(  s_axi_hp0_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_hp0->bready(  s_axi_hp0_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_hp0->bresp(   s_axi_hp0_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_hp0->buser(   s_axi_hp0_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_hp0->bid(     s_axi_hp0_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_hp0->arvalid( s_axi_hp0_arvalid );
            /* sc_out<bool>                    */ axi2tlm_hp0->arready( s_axi_hp0_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_hp0->araddr(  s_axi_hp0_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp0->arprot(  s_axi_hp0_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_hp0->aruser(  s_axi_hp0_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp0->arregion(s_axi_hp0_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp0->arqos(   s_axi_hp0_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp0->arcache( s_axi_hp0_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_hp0->arburst( s_axi_hp0_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp0->arsize(  s_axi_hp0_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_hp0->arlen(   s_axi_hp0_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_hp0->arid(    s_axi_hp0_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_hp0->arlock(  s_axi_hp0_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_hp0->rvalid(  s_axi_hp0_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_hp0->rready(  s_axi_hp0_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_hp0->rdata(   s_axi_hp0_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_hp0->rresp(   s_axi_hp0_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_hp0->ruser(   s_axi_hp0_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_hp0->rid(     s_axi_hp0_rid     );
            /* sc_out<bool>                    */ axi2tlm_hp0->rlast(   s_axi_hp0_rlast   );
        }

        // S AXI HP1
        //////////////
        if (S_AXI_HP1_ENABLE_G > 0) {
            axi2tlm_hp1 = new axi2tlm_bridge<S_AXI_HP1_ADDR_WIDTH_G,
                                             S_AXI_HP1_DATA_WIDTH_G,
                                             S_AXI_HP1_ID_WIDTH_G,
                                             S_AXI_HP1_AXLEN_WIDTH_G,
                                             S_AXI_HP1_AXLOCK_WIDTH_G,
                                             S_AXI_HP1_AWUSER_WIDTH_G,
                                             S_AXI_HP1_ARUSER_WIDTH_G> ("axi2tlm_hp1");

            zynq.s_axi_hp[1]->bind(axi2tlm_hp1->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_hp1->clk(     s_axi_hp1_aclk    );
            /* sc_in<bool>                     */ axi2tlm_hp1->resetn(  s_axi_hp1_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_hp1->awvalid( s_axi_hp1_awvalid );
            /* sc_out<bool>                    */ axi2tlm_hp1->awready( s_axi_hp1_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_hp1->awaddr(  s_axi_hp1_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp1->awprot(  s_axi_hp1_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_hp1->awuser(  s_axi_hp1_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp1->awregion(s_axi_hp1_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp1->awqos(   s_axi_hp1_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp1->awcache( s_axi_hp1_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_hp1->awburst( s_axi_hp1_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp1->awsize(  s_axi_hp1_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_hp1->awlen(   s_axi_hp1_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_hp1->awid(    s_axi_hp1_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_hp1->awlock(  s_axi_hp1_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_hp1->wid(     s_axi_hp1_wid     );
            /* sc_in<bool>                     */ axi2tlm_hp1->wvalid(  s_axi_hp1_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_hp1->wready(  s_axi_hp1_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_hp1->wdata(   s_axi_hp1_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_hp1->wstrb(   s_axi_hp1_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_hp1->wuser(   s_axi_hp1_wuser   );
            /* sc_in<bool>                     */ axi2tlm_hp1->wlast(   s_axi_hp1_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_hp1->bvalid(  s_axi_hp1_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_hp1->bready(  s_axi_hp1_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_hp1->bresp(   s_axi_hp1_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_hp1->buser(   s_axi_hp1_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_hp1->bid(     s_axi_hp1_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_hp1->arvalid( s_axi_hp1_arvalid );
            /* sc_out<bool>                    */ axi2tlm_hp1->arready( s_axi_hp1_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_hp1->araddr(  s_axi_hp1_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp1->arprot(  s_axi_hp1_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_hp1->aruser(  s_axi_hp1_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp1->arregion(s_axi_hp1_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp1->arqos(   s_axi_hp1_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp1->arcache( s_axi_hp1_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_hp1->arburst( s_axi_hp1_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp1->arsize(  s_axi_hp1_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_hp1->arlen(   s_axi_hp1_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_hp1->arid(    s_axi_hp1_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_hp1->arlock(  s_axi_hp1_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_hp1->rvalid(  s_axi_hp1_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_hp1->rready(  s_axi_hp1_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_hp1->rdata(   s_axi_hp1_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_hp1->rresp(   s_axi_hp1_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_hp1->ruser(   s_axi_hp1_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_hp1->rid(     s_axi_hp1_rid     );
            /* sc_out<bool>                    */ axi2tlm_hp1->rlast(   s_axi_hp1_rlast   );
        }

        // S AXI HP2
        //////////////
        if (S_AXI_HP2_ENABLE_G > 0) {
            axi2tlm_hp2 = new axi2tlm_bridge<S_AXI_HP2_ADDR_WIDTH_G,
                                             S_AXI_HP2_DATA_WIDTH_G,
                                             S_AXI_HP2_ID_WIDTH_G,
                                             S_AXI_HP2_AXLEN_WIDTH_G,
                                             S_AXI_HP2_AXLOCK_WIDTH_G,
                                             S_AXI_HP2_AWUSER_WIDTH_G,
                                             S_AXI_HP2_ARUSER_WIDTH_G> ("axi2tlm_hp2");

            zynq.s_axi_hp[2]->bind(axi2tlm_hp2->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_hp2->clk(     s_axi_hp2_aclk    );
            /* sc_in<bool>                     */ axi2tlm_hp2->resetn(  s_axi_hp2_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_hp2->awvalid( s_axi_hp2_awvalid );
            /* sc_out<bool>                    */ axi2tlm_hp2->awready( s_axi_hp2_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_hp2->awaddr(  s_axi_hp2_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp2->awprot(  s_axi_hp2_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_hp2->awuser(  s_axi_hp2_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp2->awregion(s_axi_hp2_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp2->awqos(   s_axi_hp2_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp2->awcache( s_axi_hp2_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_hp2->awburst( s_axi_hp2_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp2->awsize(  s_axi_hp2_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_hp2->awlen(   s_axi_hp2_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_hp2->awid(    s_axi_hp2_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_hp2->awlock(  s_axi_hp2_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_hp2->wid(     s_axi_hp2_wid     );
            /* sc_in<bool>                     */ axi2tlm_hp2->wvalid(  s_axi_hp2_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_hp2->wready(  s_axi_hp2_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_hp2->wdata(   s_axi_hp2_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_hp2->wstrb(   s_axi_hp2_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_hp2->wuser(   s_axi_hp2_wuser   );
            /* sc_in<bool>                     */ axi2tlm_hp2->wlast(   s_axi_hp2_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_hp2->bvalid(  s_axi_hp2_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_hp2->bready(  s_axi_hp2_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_hp2->bresp(   s_axi_hp2_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_hp2->buser(   s_axi_hp2_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_hp2->bid(     s_axi_hp2_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_hp2->arvalid( s_axi_hp2_arvalid );
            /* sc_out<bool>                    */ axi2tlm_hp2->arready( s_axi_hp2_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_hp2->araddr(  s_axi_hp2_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp2->arprot(  s_axi_hp2_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_hp2->aruser(  s_axi_hp2_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp2->arregion(s_axi_hp2_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp2->arqos(   s_axi_hp2_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp2->arcache( s_axi_hp2_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_hp2->arburst( s_axi_hp2_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp2->arsize(  s_axi_hp2_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_hp2->arlen(   s_axi_hp2_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_hp2->arid(    s_axi_hp2_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_hp2->arlock(  s_axi_hp2_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_hp2->rvalid(  s_axi_hp2_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_hp2->rready(  s_axi_hp2_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_hp2->rdata(   s_axi_hp2_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_hp2->rresp(   s_axi_hp2_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_hp2->ruser(   s_axi_hp2_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_hp2->rid(     s_axi_hp2_rid     );
            /* sc_out<bool>                    */ axi2tlm_hp2->rlast(   s_axi_hp2_rlast   );
        }

        // S AXI HP3
        //////////////
        if (S_AXI_HP3_ENABLE_G > 0) {
            axi2tlm_hp3 = new axi2tlm_bridge<S_AXI_HP3_ADDR_WIDTH_G,
                                             S_AXI_HP3_DATA_WIDTH_G,
                                             S_AXI_HP3_ID_WIDTH_G,
                                             S_AXI_HP3_AXLEN_WIDTH_G,
                                             S_AXI_HP3_AXLOCK_WIDTH_G,
                                             S_AXI_HP3_AWUSER_WIDTH_G,
                                             S_AXI_HP3_ARUSER_WIDTH_G> ("axi2tlm_hp3");

            zynq.s_axi_hp[3]->bind(axi2tlm_hp3->socket);

            // Note : Commented signal assignations below are connected to dummy internally (in bridge) e.g., awuser

            /* Clock and Reset */
            /* sc_in<bool>                     */ axi2tlm_hp3->clk(     s_axi_hp3_aclk    );
            /* sc_in<bool>                     */ axi2tlm_hp3->resetn(  s_axi_hp3_aresetn );

            /* Write address channel.  */
            /* sc_in<bool>                     */ axi2tlm_hp3->awvalid( s_axi_hp3_awvalid );
            /* sc_out<bool>                    */ axi2tlm_hp3->awready( s_axi_hp3_awready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_hp3->awaddr(  s_axi_hp3_awaddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp3->awprot(  s_axi_hp3_awprot  );
            /* sc_in<AXISignal(AWUSER_WIDTH) > */ //axi2tlm_hp3->awuser(  s_axi_hp3_awuser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp3->awregion(s_axi_hp3_awregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp3->awqos(   s_axi_hp3_awqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp3->awcache( s_axi_hp3_awcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_hp3->awburst( s_axi_hp3_awburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp3->awsize(  s_axi_hp3_awsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_hp3->awlen(   s_axi_hp3_awlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_hp3->awid(    s_axi_hp3_awid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_hp3->awlock(  s_axi_hp3_awlock  );

            /* Write data channel.  */
            /* sc_in<AXISignal(ID_WIDTH) >     */ //axi2tlm_hp3->wid(     s_axi_hp3_wid     );
            /* sc_in<bool>                     */ axi2tlm_hp3->wvalid(  s_axi_hp3_wvalid  );
            /* sc_out<bool>                    */ axi2tlm_hp3->wready(  s_axi_hp3_wready  );
            /* sc_in<sc_bv<DATA_WIDTH> >       */ axi2tlm_hp3->wdata(   s_axi_hp3_wdata   );
            /* sc_in<sc_bv<DATA_WIDTH/8> >     */ axi2tlm_hp3->wstrb(   s_axi_hp3_wstrb   );
            /* sc_in<AXISignal(WUSER_WIDTH) >  */ //axi2tlm_hp3->wuser(   s_axi_hp3_wuser   );
            /* sc_in<bool>                     */ axi2tlm_hp3->wlast(   s_axi_hp3_wlast   );

            /* Write response channel.  */
            /* sc_out<bool>                    */ axi2tlm_hp3->bvalid(  s_axi_hp3_bvalid  );
            /* sc_in<bool>                     */ axi2tlm_hp3->bready(  s_axi_hp3_bready  );
            /* sc_out<sc_bv<2> >               */ axi2tlm_hp3->bresp(   s_axi_hp3_bresp   );
            /* sc_out<AXISignal(BUSER_WIDTH) > */ //axi2tlm_hp3->buser(   s_axi_hp3_buser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_hp3->bid(     s_axi_hp3_bid     );

            /* Read address channel.  */
            /* sc_in<bool>                     */ axi2tlm_hp3->arvalid( s_axi_hp3_arvalid );
            /* sc_out<bool>                    */ axi2tlm_hp3->arready( s_axi_hp3_arready );
            /* sc_in<sc_bv<ADDR_WIDTH> >       */ axi2tlm_hp3->araddr(  s_axi_hp3_araddr  );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp3->arprot(  s_axi_hp3_arprot  );
            /* sc_in<AXISignal(ARUSER_WIDTH) > */ //axi2tlm_hp3->aruser(  s_axi_hp3_aruser  );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp3->arregion(s_axi_hp3_arregion);
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp3->arqos(   s_axi_hp3_arqos   );
            /* sc_in<sc_bv<4> >                */ axi2tlm_hp3->arcache( s_axi_hp3_arcache );
            /* sc_in<sc_bv<2> >                */ axi2tlm_hp3->arburst( s_axi_hp3_arburst );
            /* sc_in<sc_bv<3> >                */ axi2tlm_hp3->arsize(  s_axi_hp3_arsize  );
            /* sc_in<AXISignal(AxLEN_WIDTH) >  */ axi2tlm_hp3->arlen(   s_axi_hp3_arlen   );
            /* sc_in<AXISignal(ID_WIDTH) >     */ axi2tlm_hp3->arid(    s_axi_hp3_arid    );
            /* sc_in<AXISignal(AxLOCK_WIDTH) > */ axi2tlm_hp3->arlock(  s_axi_hp3_arlock  );

            /* Read data channel.  */
            /* sc_out<bool>                    */ axi2tlm_hp3->rvalid(  s_axi_hp3_rvalid  );
            /* sc_in<bool>                     */ axi2tlm_hp3->rready(  s_axi_hp3_rready  );
            /* sc_out<sc_bv<DATA_WIDTH> >      */ axi2tlm_hp3->rdata(   s_axi_hp3_rdata   );
            /* sc_out<sc_bv<2> >               */ axi2tlm_hp3->rresp(   s_axi_hp3_rresp   );
            /* sc_out<AXISignal(RUSER_WIDTH) > */ //axi2tlm_hp3->ruser(   s_axi_hp3_ruser   );
            /* sc_out<AXISignal(ID_WIDTH) >    */ axi2tlm_hp3->rid(     s_axi_hp3_rid     );
            /* sc_out<bool>                    */ axi2tlm_hp3->rlast(   s_axi_hp3_rlast   );
        }

zynq.tie_off();


}

SC_MODULE_EXPORT(zynq7_ps);
