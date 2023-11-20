/*
 * Top level for Zynq Cosim
 *
 * Copyright (c) 2020 REDS
 * Written by Rick Wertenbroek changed by B&R to fit with the RivieraPro
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

#define SC_INCLUDE_DYNAMIC_PROCESSES

// C/C++ includes
#include <inttypes.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <iostream>
#include <string>

// System C includes
#include "systemc.h"
// System C TLM includes
#include "tlm_utils/simple_initiator_socket.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/tlm_quantumkeeper.h"

// Xilinx Zynq
// https://github.com/Xilinx/libsystemctlm-soc in zynq (moved to soc/xilinx in future revisions)
#include "xilinx-zynq.h"

// Xilinx Bridges (BFMs)
// https://github.com/Xilinx/libsystemctlm-soc in tlm-bridges
#include "tlm2axi-bridge.h"
#include "axi2tlm-bridge.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

SC_MODULE(zynq7_ps)
{
public:

    // callback declaration
    void before_end_of_elaboration();

    //--------------------------------------------------------------------------------
    // Generics //
    //--------------------------------------------------------------------------------
    

    #ifdef __QEMU_PATH_TO_SOCKET__
        #define pathQ(x) #x
        #define PathQ(x) pathQ(x)
        std::string QEMU_PATH_TO_SOCKET_G = std::string("unix:") + std::string(PathQ(__QEMU_PATH_TO_SOCKET__));
    #else
        std::string QEMU_PATH_TO_SOCKET_G = "unix:/path/to/qemu-rport-_cosim@0";
    #endif
    
    #ifdef __QEMU_SYNC_QUANTUM_G__
        int QEMU_SYNC_QUANTUM_G = __QEMU_SYNC_QUANTUM_G__;
    #else 
        int QEMU_SYNC_QUANTUM_G = 10000;
    #endif

    int FCLK_CLK0_PERIOD_IN_NS_G = 10000;
    int FCLK_CLK1_PERIOD_IN_NS_G = 10000;
    int FCLK_CLK2_PERIOD_IN_NS_G = -1;
    int FCLK_CLK3_PERIOD_IN_NS_G = -1;
    int M_AXI_GP0_ENABLE_G = 0;
    int M_AXI_GP1_ENABLE_G = 0;
    int S_AXI_GP0_ENABLE_G = 0;
    int S_AXI_GP1_ENABLE_G = 0;
    int S_AXI_HP0_ENABLE_G = 0;
    int S_AXI_HP1_ENABLE_G = 0;
    int S_AXI_HP2_ENABLE_G = 0;
    int S_AXI_HP3_ENABLE_G = 0;

    // SystemC generics are implemented as members and can therefore not be used as template parameters
    // Templates require constant (compilation time known values and can not rely on a this->my_constant
    // type of constant apparently
#ifdef __M_AXI_GP0_ADDR_WIDTH__
    static const int M_AXI_GP0_ADDR_WIDTH_G   = __M_AXI_GP0_ADDR_WIDTH__;
#else
    static const int M_AXI_GP0_ADDR_WIDTH_G   = 32;
#endif
#ifdef __M_AXI_GP0_DATA_WIDTH__
    static const int M_AXI_GP0_DATA_WIDTH_G   = __M_AXI_GP0_DATA_WIDTH__;
#else
    static const int M_AXI_GP0_DATA_WIDTH_G   = 32;
#endif
#ifdef __M_AXI_GP0_ID_WIDTH__
    static const int M_AXI_GP0_ID_WIDTH_G     = __M_AXI_GP0_ID_WIDTH__;
#else
    static const int M_AXI_GP0_ID_WIDTH_G     = 12;
#endif
#ifdef __M_AXI_GP0_AXLEN_WIDTH__
    static const int M_AXI_GP0_AXLEN_WIDTH_G  = __M_AXI_GP0_AXLEN_WIDTH__;
#else
    static const int M_AXI_GP0_AXLEN_WIDTH_G  = 4;
#endif
#ifdef __M_AXI_GP0_AXLOCK_WIDTH__
    static const int M_AXI_GP0_AXLOCK_WIDTH_G = __M_AXI_GP0_AXLOCK_WIDTH__;
#else
    static const int M_AXI_GP0_AXLOCK_WIDTH_G = 2;
#endif
#ifdef __M_AXI_GP0_AWUSER_WIDTH__
    static const int M_AXI_GP0_AWUSER_WIDTH_G = __M_AXI_GP0_AWUSER_WIDTH__;
#else
    static const int M_AXI_GP0_AWUSER_WIDTH_G = 2;
#endif
#ifdef __M_AXI_GP0_ARUSER_WIDTH__
    static const int M_AXI_GP0_ARUSER_WIDTH_G = __M_AXI_GP0_ARUSER_WIDTH__;
#else
    static const int M_AXI_GP0_ARUSER_WIDTH_G = 2;
#endif


#ifdef __M_AXI_GP1_ADDR_WIDTH__
    static const int M_AXI_GP1_ADDR_WIDTH_G   = __M_AXI_GP1_ADDR_WIDTH__;
#else
    static const int M_AXI_GP1_ADDR_WIDTH_G   = 32;
#endif
#ifdef __M_AXI_GP1_DATA_WIDTH__
    static const int M_AXI_GP1_DATA_WIDTH_G   = __M_AXI_GP1_DATA_WIDTH__;
#else
    static const int M_AXI_GP1_DATA_WIDTH_G   = 32;
#endif
#ifdef __M_AXI_GP1_ID_WIDTH__
    static const int M_AXI_GP1_ID_WIDTH_G     = __M_AXI_GP1_ID_WIDTH__;
#else
    static const int M_AXI_GP1_ID_WIDTH_G     = 12;
#endif
#ifdef __M_AXI_GP1_AXLEN_WIDTH__
    static const int M_AXI_GP1_AXLEN_WIDTH_G  = __M_AXI_GP1_AXLEN_WIDTH__;
#else
    static const int M_AXI_GP1_AXLEN_WIDTH_G  = 4;
#endif
#ifdef __M_AXI_GP1_AXLOCK_WIDTH__
    static const int M_AXI_GP1_AXLOCK_WIDTH_G = __M_AXI_GP1_AXLOCK_WIDTH__;
#else
    static const int M_AXI_GP1_AXLOCK_WIDTH_G = 2;
#endif
#ifdef __M_AXI_GP1_AWUSER_WIDTH__
    static const int M_AXI_GP1_AWUSER_WIDTH_G = __M_AXI_GP1_AWUSER_WIDTH__;
#else
    static const int M_AXI_GP1_AWUSER_WIDTH_G = 2;
#endif
#ifdef __M_AXI_GP1_ARUSER_WIDTH__
    static const int M_AXI_GP1_ARUSER_WIDTH_G = __M_AXI_GP1_ARUSER_WIDTH__;
#else
    static const int M_AXI_GP1_ARUSER_WIDTH_G = 2;
#endif


#ifdef __S_AXI_GP0_ADDR_WIDTH__
    static const int S_AXI_GP0_ADDR_WIDTH_G   = __S_AXI_GP0_ADDR_WIDTH__;
#else
    static const int S_AXI_GP0_ADDR_WIDTH_G   = 32;
#endif
#ifdef __S_AXI_GP0_DATA_WIDTH__
    static const int S_AXI_GP0_DATA_WIDTH_G   = __S_AXI_GP0_DATA_WIDTH__;
#else
    static const int S_AXI_GP0_DATA_WIDTH_G   = 32;
#endif
#ifdef __S_AXI_GP0_ID_WIDTH__
    static const int S_AXI_GP0_ID_WIDTH_G     = __S_AXI_GP0_ID_WIDTH__;
#else
    static const int S_AXI_GP0_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_GP0_AXLEN_WIDTH__
    static const int S_AXI_GP0_AXLEN_WIDTH_G  = __S_AXI_GP0_AXLEN_WIDTH__;
#else
    static const int S_AXI_GP0_AXLEN_WIDTH_G  = 4;
#endif
#ifdef __S_AXI_GP0_AXLOCK_WIDTH__
    static const int S_AXI_GP0_AXLOCK_WIDTH_G = __S_AXI_GP0_AXLOCK_WIDTH__;
#else
    static const int S_AXI_GP0_AXLOCK_WIDTH_G = 2;
#endif
#ifdef __S_AXI_GP0_AWUSER_WIDTH__
    static const int S_AXI_GP0_AWUSER_WIDTH_G = __S_AXI_GP0_AWUSER_WIDTH__;
#else
    static const int S_AXI_GP0_AWUSER_WIDTH_G = 2;
#endif
#ifdef __S_AXI_GP0_ARUSER_WIDTH__
    static const int S_AXI_GP0_ARUSER_WIDTH_G = __S_AXI_GP0_ARUSER_WIDTH__;
#else
    static const int S_AXI_GP0_ARUSER_WIDTH_G = 2;
#endif


#ifdef __S_AXI_GP1_ADDR_WIDTH__
    static const int S_AXI_GP1_ADDR_WIDTH_G   = __S_AXI_GP1_ADDR_WIDTH__;
#else
    static const int S_AXI_GP1_ADDR_WIDTH_G   = 32;
#endif
#ifdef __S_AXI_GP1_DATA_WIDTH__
    static const int S_AXI_GP1_DATA_WIDTH_G   = __S_AXI_GP1_DATA_WIDTH__;
#else
    static const int S_AXI_GP1_DATA_WIDTH_G   = 32;
#endif
#ifdef __S_AXI_GP1_ID_WIDTH__
    static const int S_AXI_GP1_ID_WIDTH_G     = __S_AXI_GP1_ID_WIDTH__;
#else
    static const int S_AXI_GP1_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_GP1_AXLEN_WIDTH__
    static const int S_AXI_GP1_AXLEN_WIDTH_G  = __S_AXI_GP1_AXLEN_WIDTH__;
#else
    static const int S_AXI_GP1_AXLEN_WIDTH_G  = 4;
#endif
#ifdef __S_AXI_GP1_AXLOCK_WIDTH__
    static const int S_AXI_GP1_AXLOCK_WIDTH_G = __S_AXI_GP1_AXLOCK_WIDTH__;
#else
    static const int S_AXI_GP1_AXLOCK_WIDTH_G = 2;
#endif
#ifdef __S_AXI_GP1_AWUSER_WIDTH__
    static const int S_AXI_GP1_AWUSER_WIDTH_G = __S_AXI_GP1_AWUSER_WIDTH__;
#else
    static const int S_AXI_GP1_AWUSER_WIDTH_G = 2;
#endif
#ifdef __S_AXI_GP1_ARUSER_WIDTH__
    static const int S_AXI_GP1_ARUSER_WIDTH_G = __S_AXI_GP1_ARUSER_WIDTH__;
#else
    static const int S_AXI_GP1_ARUSER_WIDTH_G = 2;
#endif


#ifdef __S_AXI_HP0_ADDR_WIDTH__
    static const int S_AXI_HP0_ADDR_WIDTH_G   = __S_AXI_HP0_ADDR_WIDTH__;
#else
    static const int S_AXI_HP0_ADDR_WIDTH_G   = 32;
#endif
#ifdef __S_AXI_HP0_DATA_WIDTH__
    static const int S_AXI_HP0_DATA_WIDTH_G   = __S_AXI_HP0_DATA_WIDTH__;
#else
    static const int S_AXI_HP0_DATA_WIDTH_G   = 64;
#endif
#ifdef __S_AXI_HP0_ID_WIDTH__
    static const int S_AXI_HP0_ID_WIDTH_G     = __S_AXI_HP0_ID_WIDTH__;
#else
    static const int S_AXI_HP0_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_HP0_AXLEN_WIDTH__
    static const int S_AXI_HP0_AXLEN_WIDTH_G  = __S_AXI_HP0_AXLEN_WIDTH__;
#else
    static const int S_AXI_HP0_AXLEN_WIDTH_G  = 4;
#endif
#ifdef __S_AXI_HP0_AXLOCK_WIDTH__
    static const int S_AXI_HP0_AXLOCK_WIDTH_G = __S_AXI_HP0_AXLOCK_WIDTH__;
#else
    static const int S_AXI_HP0_AXLOCK_WIDTH_G = 2;
#endif
#ifdef __S_AXI_HP0_AWUSER_WIDTH__
    static const int S_AXI_HP0_AWUSER_WIDTH_G = __S_AXI_HP0_AWUSER_WIDTH__;
#else
    static const int S_AXI_HP0_AWUSER_WIDTH_G = 2;
#endif
#ifdef __S_AXI_HP0_ARUSER_WIDTH__
    static const int S_AXI_HP0_ARUSER_WIDTH_G = __S_AXI_HP0_ARUSER_WIDTH__;
#else
    static const int S_AXI_HP0_ARUSER_WIDTH_G = 2;
#endif


#ifdef __S_AXI_HP1_ADDR_WIDTH__
    static const int S_AXI_HP1_ADDR_WIDTH_G   = __S_AXI_HP1_ADDR_WIDTH__;
#else
    static const int S_AXI_HP1_ADDR_WIDTH_G   = 32;
#endif
#ifdef __S_AXI_HP1_DATA_WIDTH__
    static const int S_AXI_HP1_DATA_WIDTH_G   = __S_AXI_HP1_DATA_WIDTH__;
#else
    static const int S_AXI_HP1_DATA_WIDTH_G   = 64;
#endif
#ifdef __S_AXI_HP1_ID_WIDTH__
    static const int S_AXI_HP1_ID_WIDTH_G     = __S_AXI_HP1_ID_WIDTH__;
#else
    static const int S_AXI_HP1_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_HP1_AXLEN_WIDTH__
    static const int S_AXI_HP1_AXLEN_WIDTH_G  = __S_AXI_HP1_AXLEN_WIDTH__;
#else
    static const int S_AXI_HP1_AXLEN_WIDTH_G  = 4;
#endif
#ifdef __S_AXI_HP1_AXLOCK_WIDTH__
    static const int S_AXI_HP1_AXLOCK_WIDTH_G = __S_AXI_HP1_AXLOCK_WIDTH__;
#else
    static const int S_AXI_HP1_AXLOCK_WIDTH_G = 2;
#endif
#ifdef __S_AXI_HP1_AWUSER_WIDTH__
    static const int S_AXI_HP1_AWUSER_WIDTH_G = __S_AXI_HP1_AWUSER_WIDTH__;
#else
    static const int S_AXI_HP1_AWUSER_WIDTH_G = 2;
#endif
#ifdef __S_AXI_HP1_ARUSER_WIDTH__
    static const int S_AXI_HP1_ARUSER_WIDTH_G = __S_AXI_HP1_ARUSER_WIDTH__;
#else
    static const int S_AXI_HP1_ARUSER_WIDTH_G = 2;
#endif


#ifdef __S_AXI_HP2_ADDR_WIDTH__
    static const int S_AXI_HP2_ADDR_WIDTH_G   = __S_AXI_HP2_ADDR_WIDTH__;
#else
    static const int S_AXI_HP2_ADDR_WIDTH_G   = 32;
#endif
#ifdef __S_AXI_HP2_DATA_WIDTH__
    static const int S_AXI_HP2_DATA_WIDTH_G   = __S_AXI_HP2_DATA_WIDTH__;
#else
    static const int S_AXI_HP2_DATA_WIDTH_G   = 64;
#endif
#ifdef __S_AXI_HP2_ID_WIDTH__
    static const int S_AXI_HP2_ID_WIDTH_G     = __S_AXI_HP2_ID_WIDTH__;
#else
    static const int S_AXI_HP2_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_HP2_AXLEN_WIDTH__
    static const int S_AXI_HP2_AXLEN_WIDTH_G  = __S_AXI_HP2_AXLEN_WIDTH__;
#else
    static const int S_AXI_HP2_AXLEN_WIDTH_G  = 4;
#endif
#ifdef __S_AXI_HP2_AXLOCK_WIDTH__
    static const int S_AXI_HP2_AXLOCK_WIDTH_G = __S_AXI_HP2_AXLOCK_WIDTH__;
#else
    static const int S_AXI_HP2_AXLOCK_WIDTH_G = 2;
#endif
#ifdef __S_AXI_HP2_AWUSER_WIDTH__
    static const int S_AXI_HP2_AWUSER_WIDTH_G = __S_AXI_HP2_AWUSER_WIDTH__;
#else
    static const int S_AXI_HP2_AWUSER_WIDTH_G = 2;
#endif
#ifdef __S_AXI_HP2_ARUSER_WIDTH__
    static const int S_AXI_HP2_ARUSER_WIDTH_G = __S_AXI_HP2_ARUSER_WIDTH__;
#else
    static const int S_AXI_HP2_ARUSER_WIDTH_G = 2;
#endif


#ifdef __S_AXI_HP3_ADDR_WIDTH__
    static const int S_AXI_HP3_ADDR_WIDTH_G   = __S_AXI_HP3_ADDR_WIDTH__;
#else
    static const int S_AXI_HP3_ADDR_WIDTH_G   = 32;
#endif
#ifdef __S_AXI_HP3_DATA_WIDTH__
    static const int S_AXI_HP3_DATA_WIDTH_G   = __S_AXI_HP3_DATA_WIDTH__;
#else
    static const int S_AXI_HP3_DATA_WIDTH_G   = 64;
#endif
#ifdef __S_AXI_HP3_ID_WIDTH__
    static const int S_AXI_HP3_ID_WIDTH_G     = __S_AXI_HP3_ID_WIDTH__;
#else
    static const int S_AXI_HP3_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_HP3_AXLEN_WIDTH__
    static const int S_AXI_HP3_AXLEN_WIDTH_G  = __S_AXI_HP3_AXLEN_WIDTH__;
#else
    static const int S_AXI_HP3_AXLEN_WIDTH_G  = 4;
#endif
#ifdef __S_AXI_HP3_AXLOCK_WIDTH__
    static const int S_AXI_HP3_AXLOCK_WIDTH_G = __S_AXI_HP3_AXLOCK_WIDTH__;
#else
    static const int S_AXI_HP3_AXLOCK_WIDTH_G = 2;
#endif
#ifdef __S_AXI_HP3_AWUSER_WIDTH__
    static const int S_AXI_HP3_AWUSER_WIDTH_G = __S_AXI_HP3_AWUSER_WIDTH__;
#else
    static const int S_AXI_HP3_AWUSER_WIDTH_G = 2;
#endif
#ifdef __S_AXI_HP3_ARUSER_WIDTH__
    static const int S_AXI_HP3_ARUSER_WIDTH_G = __S_AXI_HP3_ARUSER_WIDTH__;
#else
    static const int S_AXI_HP3_ARUSER_WIDTH_G = 2;
#endif

    //--------------------------------------------------------------------------------
    // Module
    //--------------------------------------------------------------------------------
    xilinx_zynq zynq;

    sc_signal<bool> rst, rst_n;

    // Clocks
    ///////////
    sc_clock *fclk_clk0_gen;
    sc_clock *fclk_clk1_gen;
    sc_clock *fclk_clk2_gen;
    sc_clock *fclk_clk3_gen;
    // Clock ports
    sc_out<bool> fclk_clk0;
    sc_out<bool> fclk_clk1;
    sc_out<bool> fclk_clk2;
    sc_out<bool> fclk_clk3;
    // Clock assign functions (separate because separate sensitivities)
    inline void fclk_clk0_assign() { fclk_clk0.write(fclk_clk0_gen->signal().read()); }
    inline void fclk_clk1_assign() { fclk_clk1.write(fclk_clk1_gen->signal().read()); }
    inline void fclk_clk2_assign() { fclk_clk2.write(fclk_clk2_gen->signal().read()); }
    inline void fclk_clk3_assign() { fclk_clk3.write(fclk_clk3_gen->signal().read()); }

    inline void fclk_clk1_disable() { fclk_clk1.write(false); }
    inline void fclk_clk2_disable() { fclk_clk2.write(false); }
    inline void fclk_clk3_disable() { fclk_clk3.write(false); }






    /////////
    // AXI //
    /////////

    // Bridges
    ////////////

    // Pointer to bridge for M AXI GP0
    tlm2axi_bridge<M_AXI_GP0_ADDR_WIDTH_G,
                   M_AXI_GP0_DATA_WIDTH_G,
                   M_AXI_GP0_ID_WIDTH_G,
                   M_AXI_GP0_AXLEN_WIDTH_G,
                   M_AXI_GP0_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0
                    > *tlm2axi_gp0;

    // Pointer to bridge for M AXI GP1
    tlm2axi_bridge<M_AXI_GP1_ADDR_WIDTH_G,
                   M_AXI_GP1_DATA_WIDTH_G,
                   M_AXI_GP1_ID_WIDTH_G,
                   M_AXI_GP1_AXLEN_WIDTH_G,
                   M_AXI_GP1_AXLOCK_WIDTH_G,
                   M_AXI_GP1_AWUSER_WIDTH_G,
                   M_AXI_GP1_ARUSER_WIDTH_G> *tlm2axi_gp1;

    // Pointer to bridge for S AXI GP0
    axi2tlm_bridge<S_AXI_GP0_ADDR_WIDTH_G,
                   S_AXI_GP0_DATA_WIDTH_G,
                   S_AXI_GP0_ID_WIDTH_G,
                   S_AXI_GP0_AXLEN_WIDTH_G,
                   S_AXI_GP0_AXLOCK_WIDTH_G,
                   S_AXI_GP0_AWUSER_WIDTH_G,
                   S_AXI_GP0_ARUSER_WIDTH_G> *axi2tlm_gp0;

    // Pointer to bridge for S AXI GP1
    axi2tlm_bridge<S_AXI_GP1_ADDR_WIDTH_G,
                   S_AXI_GP1_DATA_WIDTH_G,
                   S_AXI_GP1_ID_WIDTH_G,
                   S_AXI_GP1_AXLEN_WIDTH_G,
                   S_AXI_GP1_AXLOCK_WIDTH_G,
                   S_AXI_GP1_AWUSER_WIDTH_G,
                   S_AXI_GP1_ARUSER_WIDTH_G> *axi2tlm_gp1;

    // Pointer to bridge for S AXI HP0
    axi2tlm_bridge<S_AXI_HP0_ADDR_WIDTH_G,
                   S_AXI_HP0_DATA_WIDTH_G,
                   S_AXI_HP0_ID_WIDTH_G,
                   S_AXI_HP0_AXLEN_WIDTH_G,
                   S_AXI_HP0_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0> *axi2tlm_hp0;

    // Pointer to bridge for S AXI HP1
    axi2tlm_bridge<S_AXI_HP1_ADDR_WIDTH_G,
                   S_AXI_HP1_DATA_WIDTH_G,
                   S_AXI_HP1_ID_WIDTH_G,
                   S_AXI_HP1_AXLEN_WIDTH_G,
                   S_AXI_HP1_AXLOCK_WIDTH_G,
                   S_AXI_HP1_AWUSER_WIDTH_G,
                   S_AXI_HP1_ARUSER_WIDTH_G> *axi2tlm_hp1;

    // Pointer to bridge for S AXI HP2
    axi2tlm_bridge<S_AXI_HP2_ADDR_WIDTH_G,
                   S_AXI_HP2_DATA_WIDTH_G,
                   S_AXI_HP2_ID_WIDTH_G,
                   S_AXI_HP2_AXLEN_WIDTH_G,
                   S_AXI_HP2_AXLOCK_WIDTH_G,
                   S_AXI_HP2_AWUSER_WIDTH_G,
                   S_AXI_HP2_ARUSER_WIDTH_G> *axi2tlm_hp2;

    // Pointer to bridge for S AXI HP3
    axi2tlm_bridge<S_AXI_HP3_ADDR_WIDTH_G,
                   S_AXI_HP3_DATA_WIDTH_G,
                   S_AXI_HP3_ID_WIDTH_G,
                   S_AXI_HP3_AXLEN_WIDTH_G,
                   S_AXI_HP3_AXLOCK_WIDTH_G,
                   S_AXI_HP3_AWUSER_WIDTH_G,
                   S_AXI_HP3_ARUSER_WIDTH_G> *axi2tlm_hp3;

    // M AXI GP0 Ports
    ////////////////////

    sc_in<bool>                              m_axi_gp0_aclk;
    sc_in<bool>                              m_axi_gp0_aresetn;

    /* Write address channel.  */
    sc_out<bool >                            m_axi_gp0_awvalid;
    sc_in<bool >                             m_axi_gp0_awready;
    sc_out<sc_bv<M_AXI_GP0_ADDR_WIDTH_G> >   m_axi_gp0_awaddr;
    sc_out<sc_bv<3> >                        m_axi_gp0_awprot;
    sc_out<sc_bv<4> >                        m_axi_gp0_awqos;
    sc_out<sc_bv<4> >                        m_axi_gp0_awcache;
    sc_out<sc_bv<2> >                        m_axi_gp0_awburst;
    sc_out<sc_bv<3> >                        m_axi_gp0_awsize;
    sc_out<sc_bv<M_AXI_GP0_AXLEN_WIDTH_G> >  m_axi_gp0_awlen;
    sc_out<sc_bv<M_AXI_GP0_ID_WIDTH_G> >     m_axi_gp0_awid;
    sc_out<sc_bv<M_AXI_GP0_AXLOCK_WIDTH_G> > m_axi_gp0_awlock;
    // BFM side is shown below : so some modification may be needed for these signals
    // Each time BFM side differs is shown in comments below
    //sc_out<AXISignal(AxLEN_WIDTH) > awlen;
    //sc_out<AXISignal(ID_WIDTH) > awid;
    //sc_out<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_out<sc_bv<M_AXI_GP0_ID_WIDTH_G> >     m_axi_gp0_wid;
    //sc_out<AXISignal(ID_WIDTH) > wid;
    sc_out<bool >                            m_axi_gp0_wvalid;
    sc_in<bool >                             m_axi_gp0_wready;
    sc_out<sc_bv<M_AXI_GP0_DATA_WIDTH_G> >   m_axi_gp0_wdata;
    sc_out<sc_bv<M_AXI_GP0_DATA_WIDTH_G/8> > m_axi_gp0_wstrb;
    sc_out<bool >                            m_axi_gp0_wlast;

    /* Write response channel.  */
    sc_in<bool >                             m_axi_gp0_bvalid;
    sc_out<bool >                            m_axi_gp0_bready;
    sc_in<sc_bv<2> >                         m_axi_gp0_bresp;
    //sc_in<AXISignal(BUSER_WIDTH) > buser; // UNUSED so tied-off
    sc_in<sc_bv<M_AXI_GP0_ID_WIDTH_G> >      m_axi_gp0_bid;
    //sc_in<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_out<bool >                            m_axi_gp0_arvalid;
    sc_in<bool >                             m_axi_gp0_arready;
    sc_out<sc_bv<M_AXI_GP0_ADDR_WIDTH_G> >   m_axi_gp0_araddr;
    sc_out<sc_bv<3> >                        m_axi_gp0_arprot;
    sc_out<sc_bv<4> >                        m_axi_gp0_arqos;
    sc_out<sc_bv<4> >                        m_axi_gp0_arcache;
    sc_out<sc_bv<2> >                        m_axi_gp0_arburst;
    sc_out<sc_bv<3> >                        m_axi_gp0_arsize;
    sc_out<sc_bv<M_AXI_GP0_AXLEN_WIDTH_G> >  m_axi_gp0_arlen;
    sc_out<sc_bv<M_AXI_GP0_ID_WIDTH_G> >     m_axi_gp0_arid;
    sc_out<sc_bv<M_AXI_GP0_AXLOCK_WIDTH_G> > m_axi_gp0_arlock;
    //sc_out<AXISignal(AxLEN_WIDTH) > arlen;
    //sc_out<AXISignal(ID_WIDTH) > arid;
    //sc_out<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_in<bool >                             m_axi_gp0_rvalid;
    sc_out<bool >                            m_axi_gp0_rready;
    sc_in<sc_bv<M_AXI_GP0_DATA_WIDTH_G> >    m_axi_gp0_rdata;
    sc_in<sc_bv<2> >                         m_axi_gp0_rresp;
    //sc_in<AXISignal(RUSER_WIDTH) > ruser; // UNUSED so tied-off
    sc_in<sc_bv<M_AXI_GP0_ID_WIDTH_G> >      m_axi_gp0_rid;
    //sc_in<AXISignal(ID_WIDTH) > rid;
    sc_in<bool >                             m_axi_gp0_rlast;

    // Ports so that the bridge can be bound
    sc_out<sc_bv<4> > m_axi_gp0_arregion;
    sc_out<sc_bv<4> > m_axi_gp0_awregion;

    // M AXI GP1 Ports
    ////////////////////

    sc_in<bool>                              m_axi_gp1_aclk;
    sc_in<bool>                              m_axi_gp1_aresetn;

    /* Write address channel.  */
    sc_out<bool >                            m_axi_gp1_awvalid;
    sc_in<bool >                             m_axi_gp1_awready;
    sc_out<sc_bv<M_AXI_GP1_ADDR_WIDTH_G> >   m_axi_gp1_awaddr;
    sc_out<sc_bv<3> >                        m_axi_gp1_awprot;
    sc_out<sc_bv<4> >                        m_axi_gp1_awqos;
    sc_out<sc_bv<4> >                        m_axi_gp1_awcache;
    sc_out<sc_bv<2> >                        m_axi_gp1_awburst;
    sc_out<sc_bv<3> >                        m_axi_gp1_awsize;
    sc_out<sc_bv<M_AXI_GP1_AXLEN_WIDTH_G> >  m_axi_gp1_awlen;
    sc_out<sc_bv<M_AXI_GP1_ID_WIDTH_G> >     m_axi_gp1_awid;
    sc_out<sc_bv<M_AXI_GP1_AXLOCK_WIDTH_G> > m_axi_gp1_awlock;
    // BFM side is shown below : so some modification may be needed for these signals
    // Each time BFM side differs is shown in comments below
    //sc_out<AXISignal(AxLEN_WIDTH) > awlen;
    //sc_out<AXISignal(ID_WIDTH) > awid;
    //sc_out<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_out<sc_bv<M_AXI_GP1_ID_WIDTH_G> >     m_axi_gp1_wid;
    //sc_out<AXISignal(ID_WIDTH) > wid;
    sc_out<bool >                            m_axi_gp1_wvalid;
    sc_in<bool >                             m_axi_gp1_wready;
    sc_out<sc_bv<M_AXI_GP1_DATA_WIDTH_G> >   m_axi_gp1_wdata;
    sc_out<sc_bv<M_AXI_GP1_DATA_WIDTH_G/8> > m_axi_gp1_wstrb;
    sc_out<bool >                            m_axi_gp1_wlast;

    /* Write response channel.  */
    sc_in<bool >                             m_axi_gp1_bvalid;
    sc_out<bool >                            m_axi_gp1_bready;
    sc_in<sc_bv<2> >                         m_axi_gp1_bresp;
    //sc_in<AXISignal(BUSER_WIDTH) > buser; // UNUSED so tied-off
    sc_in<sc_bv<M_AXI_GP1_ID_WIDTH_G> >      m_axi_gp1_bid;
    //sc_in<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_out<bool >                            m_axi_gp1_arvalid;
    sc_in<bool >                             m_axi_gp1_arready;
    sc_out<sc_bv<M_AXI_GP1_ADDR_WIDTH_G> >   m_axi_gp1_araddr;
    sc_out<sc_bv<3> >                        m_axi_gp1_arprot;
    sc_out<sc_bv<4> >                        m_axi_gp1_arqos;
    sc_out<sc_bv<4> >                        m_axi_gp1_arcache;
    sc_out<sc_bv<2> >                        m_axi_gp1_arburst;
    sc_out<sc_bv<3> >                        m_axi_gp1_arsize;
    sc_out<sc_bv<M_AXI_GP1_AXLEN_WIDTH_G> >  m_axi_gp1_arlen;
    sc_out<sc_bv<M_AXI_GP1_ID_WIDTH_G> >     m_axi_gp1_arid;
    sc_out<sc_bv<M_AXI_GP1_AXLOCK_WIDTH_G> > m_axi_gp1_arlock;
    //sc_out<AXISignal(AxLEN_WIDTH) > arlen;
    //sc_out<AXISignal(ID_WIDTH) > arid;
    //sc_out<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_in<bool >                             m_axi_gp1_rvalid;
    sc_out<bool >                            m_axi_gp1_rready;
    sc_in<sc_bv<M_AXI_GP1_DATA_WIDTH_G> >    m_axi_gp1_rdata;
    sc_in<sc_bv<2> >                         m_axi_gp1_rresp;
    //sc_in<AXISignal(RUSER_WIDTH) > ruser; // UNUSED so tied-off
    sc_in<sc_bv<M_AXI_GP1_ID_WIDTH_G> >      m_axi_gp1_rid;
    //sc_in<AXISignal(ID_WIDTH) > rid;
    sc_in<bool >                             m_axi_gp1_rlast;

    // Ports so that the bridge can be bound
    sc_out<sc_bv<4> > m_axi_gp1_arregion;
    sc_out<sc_bv<4> > m_axi_gp1_awregion;

    // S AXI GP0 Ports
    ////////////////////
    sc_in<bool>                              s_axi_gp0_aclk;
    sc_in<bool>                              s_axi_gp0_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              s_axi_gp0_awvalid;
    sc_out<bool>                             s_axi_gp0_awready;
    sc_in<sc_bv<S_AXI_GP0_ADDR_WIDTH_G> >    s_axi_gp0_awaddr;
    sc_in<sc_bv<3> >                         s_axi_gp0_awprot;
    //sc_in<sc_bv<S_AXI_GP0_AWUSER_WIDTH_G> >  s_axi_gp0_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         s_axi_gp0_awregion;
    sc_in<sc_bv<4> >                         s_axi_gp0_awqos;
    sc_in<sc_bv<4> >                         s_axi_gp0_awcache;
    sc_in<sc_bv<2> >                         s_axi_gp0_awburst;
    sc_in<sc_bv<3> >                         s_axi_gp0_awsize;
    sc_in<sc_bv<S_AXI_GP0_AXLEN_WIDTH_G> >   s_axi_gp0_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_GP0_ID_WIDTH_G> >      s_axi_gp0_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    sc_in<sc_bv<S_AXI_GP0_AXLOCK_WIDTH_G> >  s_axi_gp0_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_GP0_ID_WIDTH_G> >      s_axi_gp0_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              s_axi_gp0_wvalid;
    sc_out<bool>                             s_axi_gp0_wready;
    sc_in<sc_bv<S_AXI_GP0_DATA_WIDTH_G> >    s_axi_gp0_wdata;
    sc_in<sc_bv<S_AXI_GP0_DATA_WIDTH_G/8> >  s_axi_gp0_wstrb;
    //sc_in<sc_bv<2> >                         s_axi_gp0_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              s_axi_gp0_wlast;

    /* Write response channel.  */
    sc_out<bool>                             s_axi_gp0_bvalid;
    sc_in<bool>                              s_axi_gp0_bready;
    sc_out<sc_bv<2> >                        s_axi_gp0_bresp;
    //sc_out<sc_bv<2> >                        s_axi_gp0_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_GP0_ID_WIDTH_G> >     s_axi_gp0_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              s_axi_gp0_arvalid;
    sc_out<bool>                             s_axi_gp0_arready;
    sc_in<sc_bv<S_AXI_GP0_ADDR_WIDTH_G> >    s_axi_gp0_araddr;
    sc_in<sc_bv<3> >                         s_axi_gp0_arprot;
    //sc_in<sc_bv<S_AXI_GP0_ARUSER_WIDTH_G> >  s_axi_gp0_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         s_axi_gp0_arregion;
    sc_in<sc_bv<4> >                         s_axi_gp0_arqos;
    sc_in<sc_bv<4> >                         s_axi_gp0_arcache;
    sc_in<sc_bv<2> >                         s_axi_gp0_arburst;
    sc_in<sc_bv<3> >                         s_axi_gp0_arsize;
    sc_in<sc_bv<S_AXI_GP0_AXLEN_WIDTH_G> >   s_axi_gp0_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_GP0_ID_WIDTH_G> >      s_axi_gp0_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    sc_in<sc_bv<S_AXI_GP0_AXLOCK_WIDTH_G> >  s_axi_gp0_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             s_axi_gp0_rvalid;
    sc_in<bool>                              s_axi_gp0_rready;
    sc_out<sc_bv<S_AXI_GP0_DATA_WIDTH_G> >   s_axi_gp0_rdata;
    sc_out<sc_bv<2> >                        s_axi_gp0_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        s_axi_gp0_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_GP0_ID_WIDTH_G> >     s_axi_gp0_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             s_axi_gp0_rlast;

    // S AXI GP1 Ports
    ////////////////////
    sc_in<bool>                              s_axi_gp1_aclk;
    sc_in<bool>                              s_axi_gp1_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              s_axi_gp1_awvalid;
    sc_out<bool>                             s_axi_gp1_awready;
    sc_in<sc_bv<S_AXI_GP1_ADDR_WIDTH_G> >    s_axi_gp1_awaddr;
    sc_in<sc_bv<3> >                         s_axi_gp1_awprot;
    //sc_in<sc_bv<S_AXI_GP1_AWUSER_WIDTH_G> >  s_axi_gp1_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         s_axi_gp1_awregion;
    sc_in<sc_bv<4> >                         s_axi_gp1_awqos;
    sc_in<sc_bv<4> >                         s_axi_gp1_awcache;
    sc_in<sc_bv<2> >                         s_axi_gp1_awburst;
    sc_in<sc_bv<3> >                         s_axi_gp1_awsize;
    sc_in<sc_bv<S_AXI_GP1_AXLEN_WIDTH_G> >   s_axi_gp1_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_GP1_ID_WIDTH_G> >      s_axi_gp1_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    sc_in<sc_bv<S_AXI_GP1_AXLOCK_WIDTH_G> >  s_axi_gp1_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_GP1_ID_WIDTH_G> >      s_axi_gp1_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              s_axi_gp1_wvalid;
    sc_out<bool>                             s_axi_gp1_wready;
    sc_in<sc_bv<S_AXI_GP1_DATA_WIDTH_G> >    s_axi_gp1_wdata;
    sc_in<sc_bv<S_AXI_GP1_DATA_WIDTH_G/8> >  s_axi_gp1_wstrb;
    //sc_in<sc_bv<2> >                         s_axi_gp1_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              s_axi_gp1_wlast;

    /* Write response channel.  */
    sc_out<bool>                             s_axi_gp1_bvalid;
    sc_in<bool>                              s_axi_gp1_bready;
    sc_out<sc_bv<2> >                        s_axi_gp1_bresp;
    //sc_out<sc_bv<2> >                        s_axi_gp1_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_GP1_ID_WIDTH_G> >     s_axi_gp1_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              s_axi_gp1_arvalid;
    sc_out<bool>                             s_axi_gp1_arready;
    sc_in<sc_bv<S_AXI_GP1_ADDR_WIDTH_G> >    s_axi_gp1_araddr;
    sc_in<sc_bv<3> >                         s_axi_gp1_arprot;
    //sc_in<sc_bv<S_AXI_GP1_ARUSER_WIDTH_G> >  s_axi_gp1_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         s_axi_gp1_arregion;
    sc_in<sc_bv<4> >                         s_axi_gp1_arqos;
    sc_in<sc_bv<4> >                         s_axi_gp1_arcache;
    sc_in<sc_bv<2> >                         s_axi_gp1_arburst;
    sc_in<sc_bv<3> >                         s_axi_gp1_arsize;
    sc_in<sc_bv<S_AXI_GP1_AXLEN_WIDTH_G> >   s_axi_gp1_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_GP1_ID_WIDTH_G> >      s_axi_gp1_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    sc_in<sc_bv<S_AXI_GP1_AXLOCK_WIDTH_G> >  s_axi_gp1_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             s_axi_gp1_rvalid;
    sc_in<bool>                              s_axi_gp1_rready;
    sc_out<sc_bv<S_AXI_GP1_DATA_WIDTH_G> >   s_axi_gp1_rdata;
    sc_out<sc_bv<2> >                        s_axi_gp1_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        s_axi_gp1_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_GP1_ID_WIDTH_G> >     s_axi_gp1_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             s_axi_gp1_rlast;

    // S AXI HP0 Ports
    ////////////////////

    sc_in<bool>                              s_axi_hp0_aclk;
    sc_in<bool>                              s_axi_hp0_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              s_axi_hp0_awvalid;
    sc_out<bool>                             s_axi_hp0_awready;
    sc_in<sc_bv<S_AXI_HP0_ADDR_WIDTH_G> >    s_axi_hp0_awaddr;
    sc_in<sc_bv<3> >                         s_axi_hp0_awprot;
    //sc_in<sc_bv<S_AXI_HP0_AWUSER_WIDTH_G> >  s_axi_hp0_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         s_axi_hp0_awregion;
    sc_in<sc_bv<4> >                         s_axi_hp0_awqos;
    sc_in<sc_bv<4> >                         s_axi_hp0_awcache;
    sc_in<sc_bv<2> >                         s_axi_hp0_awburst;
    sc_in<sc_bv<3> >                         s_axi_hp0_awsize;
    sc_in<sc_bv<S_AXI_HP0_AXLEN_WIDTH_G> >   s_axi_hp0_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_HP0_ID_WIDTH_G> >      s_axi_hp0_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    sc_in<sc_bv<S_AXI_HP0_AXLOCK_WIDTH_G> >  s_axi_hp0_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_HP0_ID_WIDTH_G> >      s_axi_hp0_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              s_axi_hp0_wvalid;
    sc_out<bool>                             s_axi_hp0_wready;
    sc_in<sc_bv<S_AXI_HP0_DATA_WIDTH_G> >    s_axi_hp0_wdata;
    sc_in<sc_bv<S_AXI_HP0_DATA_WIDTH_G/8> >  s_axi_hp0_wstrb;
    //sc_in<sc_bv<2> >                         s_axi_hp0_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              s_axi_hp0_wlast;

    /* Write response channel.  */
    sc_out<bool>                             s_axi_hp0_bvalid;
    sc_in<bool>                              s_axi_hp0_bready;
    sc_out<sc_bv<2> >                        s_axi_hp0_bresp;
    //sc_out<sc_bv<2> >                        s_axi_hp0_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_HP0_ID_WIDTH_G> >     s_axi_hp0_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              s_axi_hp0_arvalid;
    sc_out<bool>                             s_axi_hp0_arready;
    sc_in<sc_bv<S_AXI_HP0_ADDR_WIDTH_G> >    s_axi_hp0_araddr;
    sc_in<sc_bv<3> >                         s_axi_hp0_arprot;
    //sc_in<sc_bv<S_AXI_HP0_ARUSER_WIDTH_G> >  s_axi_hp0_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         s_axi_hp0_arregion;
    sc_in<sc_bv<4> >                         s_axi_hp0_arqos;
    sc_in<sc_bv<4> >                         s_axi_hp0_arcache;
    sc_in<sc_bv<2> >                         s_axi_hp0_arburst;
    sc_in<sc_bv<3> >                         s_axi_hp0_arsize;
    sc_in<sc_bv<S_AXI_HP0_AXLEN_WIDTH_G> >   s_axi_hp0_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_HP0_ID_WIDTH_G> >      s_axi_hp0_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    sc_in<sc_bv<S_AXI_HP0_AXLOCK_WIDTH_G> >  s_axi_hp0_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             s_axi_hp0_rvalid;
    sc_in<bool>                              s_axi_hp0_rready;
    sc_out<sc_bv<S_AXI_HP0_DATA_WIDTH_G> >   s_axi_hp0_rdata;
    sc_out<sc_bv<2> >                        s_axi_hp0_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        s_axi_hp0_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_HP0_ID_WIDTH_G> >     s_axi_hp0_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             s_axi_hp0_rlast;

    // S AXI HP1 Ports
    ////////////////////

    sc_in<bool>                              s_axi_hp1_aclk;
    sc_in<bool>                              s_axi_hp1_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              s_axi_hp1_awvalid;
    sc_out<bool>                             s_axi_hp1_awready;
    sc_in<sc_bv<S_AXI_HP1_ADDR_WIDTH_G> >    s_axi_hp1_awaddr;
    sc_in<sc_bv<3> >                         s_axi_hp1_awprot;
    //sc_in<sc_bv<S_AXI_HP1_AWUSER_WIDTH_G> >  s_axi_hp1_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         s_axi_hp1_awregion;
    sc_in<sc_bv<4> >                         s_axi_hp1_awqos;
    sc_in<sc_bv<4> >                         s_axi_hp1_awcache;
    sc_in<sc_bv<2> >                         s_axi_hp1_awburst;
    sc_in<sc_bv<3> >                         s_axi_hp1_awsize;
    sc_in<sc_bv<S_AXI_HP1_AXLEN_WIDTH_G> >   s_axi_hp1_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_HP1_ID_WIDTH_G> >      s_axi_hp1_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    sc_in<sc_bv<S_AXI_HP1_AXLOCK_WIDTH_G> >  s_axi_hp1_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_HP1_ID_WIDTH_G> >      s_axi_hp1_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              s_axi_hp1_wvalid;
    sc_out<bool>                             s_axi_hp1_wready;
    sc_in<sc_bv<S_AXI_HP1_DATA_WIDTH_G> >    s_axi_hp1_wdata;
    sc_in<sc_bv<S_AXI_HP1_DATA_WIDTH_G/8> >  s_axi_hp1_wstrb;
    //sc_in<sc_bv<2> >                         s_axi_hp1_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              s_axi_hp1_wlast;

    /* Write response channel.  */
    sc_out<bool>                             s_axi_hp1_bvalid;
    sc_in<bool>                              s_axi_hp1_bready;
    sc_out<sc_bv<2> >                        s_axi_hp1_bresp;
    //sc_out<sc_bv<2> >                        s_axi_hp1_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_HP1_ID_WIDTH_G> >     s_axi_hp1_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              s_axi_hp1_arvalid;
    sc_out<bool>                             s_axi_hp1_arready;
    sc_in<sc_bv<S_AXI_HP1_ADDR_WIDTH_G> >    s_axi_hp1_araddr;
    sc_in<sc_bv<3> >                         s_axi_hp1_arprot;
    //sc_in<sc_bv<S_AXI_HP1_ARUSER_WIDTH_G> >  s_axi_hp1_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         s_axi_hp1_arregion;
    sc_in<sc_bv<4> >                         s_axi_hp1_arqos;
    sc_in<sc_bv<4> >                         s_axi_hp1_arcache;
    sc_in<sc_bv<2> >                         s_axi_hp1_arburst;
    sc_in<sc_bv<3> >                         s_axi_hp1_arsize;
    sc_in<sc_bv<S_AXI_HP1_AXLEN_WIDTH_G> >   s_axi_hp1_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_HP1_ID_WIDTH_G> >      s_axi_hp1_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    sc_in<sc_bv<S_AXI_HP1_AXLOCK_WIDTH_G> >  s_axi_hp1_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             s_axi_hp1_rvalid;
    sc_in<bool>                              s_axi_hp1_rready;
    sc_out<sc_bv<S_AXI_HP1_DATA_WIDTH_G> >   s_axi_hp1_rdata;
    sc_out<sc_bv<2> >                        s_axi_hp1_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        s_axi_hp1_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_HP1_ID_WIDTH_G> >     s_axi_hp1_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             s_axi_hp1_rlast;

    // S AXI HP2 Ports
    ////////////////////

    sc_in<bool>                              s_axi_hp2_aclk;
    sc_in<bool>                              s_axi_hp2_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              s_axi_hp2_awvalid;
    sc_out<bool>                             s_axi_hp2_awready;
    sc_in<sc_bv<S_AXI_HP2_ADDR_WIDTH_G> >    s_axi_hp2_awaddr;
    sc_in<sc_bv<3> >                         s_axi_hp2_awprot;
    //sc_in<sc_bv<S_AXI_HP2_AWUSER_WIDTH_G> >  s_axi_hp2_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         s_axi_hp2_awregion;
    sc_in<sc_bv<4> >                         s_axi_hp2_awqos;
    sc_in<sc_bv<4> >                         s_axi_hp2_awcache;
    sc_in<sc_bv<2> >                         s_axi_hp2_awburst;
    sc_in<sc_bv<3> >                         s_axi_hp2_awsize;
    sc_in<sc_bv<S_AXI_HP2_AXLEN_WIDTH_G> >   s_axi_hp2_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_HP2_ID_WIDTH_G> >      s_axi_hp2_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    sc_in<sc_bv<S_AXI_HP2_AXLOCK_WIDTH_G> >  s_axi_hp2_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_HP2_ID_WIDTH_G> >      s_axi_hp2_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              s_axi_hp2_wvalid;
    sc_out<bool>                             s_axi_hp2_wready;
    sc_in<sc_bv<S_AXI_HP2_DATA_WIDTH_G> >    s_axi_hp2_wdata;
    sc_in<sc_bv<S_AXI_HP2_DATA_WIDTH_G/8> >  s_axi_hp2_wstrb;
    //sc_in<sc_bv<2> >                         s_axi_hp2_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              s_axi_hp2_wlast;

    /* Write response channel.  */
    sc_out<bool>                             s_axi_hp2_bvalid;
    sc_in<bool>                              s_axi_hp2_bready;
    sc_out<sc_bv<2> >                        s_axi_hp2_bresp;
    //sc_out<sc_bv<2> >                        s_axi_hp2_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_HP2_ID_WIDTH_G> >     s_axi_hp2_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              s_axi_hp2_arvalid;
    sc_out<bool>                             s_axi_hp2_arready;
    sc_in<sc_bv<S_AXI_HP2_ADDR_WIDTH_G> >    s_axi_hp2_araddr;
    sc_in<sc_bv<3> >                         s_axi_hp2_arprot;
    //sc_in<sc_bv<S_AXI_HP2_ARUSER_WIDTH_G> >  s_axi_hp2_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         s_axi_hp2_arregion;
    sc_in<sc_bv<4> >                         s_axi_hp2_arqos;
    sc_in<sc_bv<4> >                         s_axi_hp2_arcache;
    sc_in<sc_bv<2> >                         s_axi_hp2_arburst;
    sc_in<sc_bv<3> >                         s_axi_hp2_arsize;
    sc_in<sc_bv<S_AXI_HP2_AXLEN_WIDTH_G> >   s_axi_hp2_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_HP2_ID_WIDTH_G> >      s_axi_hp2_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    sc_in<sc_bv<S_AXI_HP2_AXLOCK_WIDTH_G> >  s_axi_hp2_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             s_axi_hp2_rvalid;
    sc_in<bool>                              s_axi_hp2_rready;
    sc_out<sc_bv<S_AXI_HP2_DATA_WIDTH_G> >   s_axi_hp2_rdata;
    sc_out<sc_bv<2> >                        s_axi_hp2_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        s_axi_hp2_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_HP2_ID_WIDTH_G> >     s_axi_hp2_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             s_axi_hp2_rlast;

    // S AXI HP3 Ports
    ////////////////////

    sc_in<bool>                              s_axi_hp3_aclk;
    sc_in<bool>                              s_axi_hp3_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              s_axi_hp3_awvalid;
    sc_out<bool>                             s_axi_hp3_awready;
    sc_in<sc_bv<S_AXI_HP3_ADDR_WIDTH_G> >    s_axi_hp3_awaddr;
    sc_in<sc_bv<3> >                         s_axi_hp3_awprot;
    //sc_in<sc_bv<S_AXI_HP3_AWUSER_WIDTH_G> >  s_axi_hp3_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         s_axi_hp3_awregion;
    sc_in<sc_bv<4> >                         s_axi_hp3_awqos;
    sc_in<sc_bv<4> >                         s_axi_hp3_awcache;
    sc_in<sc_bv<2> >                         s_axi_hp3_awburst;
    sc_in<sc_bv<3> >                         s_axi_hp3_awsize;
    sc_in<sc_bv<S_AXI_HP3_AXLEN_WIDTH_G> >   s_axi_hp3_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_HP3_ID_WIDTH_G> >      s_axi_hp3_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    sc_in<sc_bv<S_AXI_HP3_AXLOCK_WIDTH_G> >  s_axi_hp3_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_HP3_ID_WIDTH_G> >      s_axi_hp3_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              s_axi_hp3_wvalid;
    sc_out<bool>                             s_axi_hp3_wready;
    sc_in<sc_bv<S_AXI_HP3_DATA_WIDTH_G> >    s_axi_hp3_wdata;
    sc_in<sc_bv<S_AXI_HP3_DATA_WIDTH_G/8> >  s_axi_hp3_wstrb;
    //sc_in<sc_bv<2> >                         s_axi_hp3_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              s_axi_hp3_wlast;

    /* Write response channel.  */
    sc_out<bool>                             s_axi_hp3_bvalid;
    sc_in<bool>                              s_axi_hp3_bready;
    sc_out<sc_bv<2> >                        s_axi_hp3_bresp;
    //sc_out<sc_bv<2> >                        s_axi_hp3_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_HP3_ID_WIDTH_G> >     s_axi_hp3_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              s_axi_hp3_arvalid;
    sc_out<bool>                             s_axi_hp3_arready;
    sc_in<sc_bv<S_AXI_HP3_ADDR_WIDTH_G> >    s_axi_hp3_araddr;
    sc_in<sc_bv<3> >                         s_axi_hp3_arprot;
    //sc_in<sc_bv<S_AXI_HP3_ARUSER_WIDTH_G> >  s_axi_hp3_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         s_axi_hp3_arregion;
    sc_in<sc_bv<4> >                         s_axi_hp3_arqos;
    sc_in<sc_bv<4> >                         s_axi_hp3_arcache;
    sc_in<sc_bv<2> >                         s_axi_hp3_arburst;
    sc_in<sc_bv<3> >                         s_axi_hp3_arsize;
    sc_in<sc_bv<S_AXI_HP3_AXLEN_WIDTH_G> >   s_axi_hp3_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_HP3_ID_WIDTH_G> >      s_axi_hp3_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    sc_in<sc_bv<S_AXI_HP3_AXLOCK_WIDTH_G> >  s_axi_hp3_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             s_axi_hp3_rvalid;
    sc_in<bool>                              s_axi_hp3_rready;
    sc_out<sc_bv<S_AXI_HP3_DATA_WIDTH_G> >   s_axi_hp3_rdata;
    sc_out<sc_bv<2> >                        s_axi_hp3_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        s_axi_hp3_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_HP3_ID_WIDTH_G> >     s_axi_hp3_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             s_axi_hp3_rlast;

    ///////// END AXI


    // IRQs
    /////////

    // PL -> PS IRQ
    sc_in<sc_bv<16> > irq_f2p;

    // Resets
    ///////////

    // Global reset
    sc_out<bool> rst_o;

    /////////////
    // Methods //
    /////////////

    // Initial Reset
    void pull_reset(void) {
        /* Pull the reset signal.  */
        rst.write(true);
        rst_o.write(true);
        //nrst_sti.write(SC_LOGIC_0);
        wait(1000, SC_US);
        rst.write(false);
        rst_o.write(false);
    }

    // Assignations of IRQs
    void assign_irq(void) {
        auto tmp_irq = irq_f2p.read();
        for (int i = 0; i < 16; ++i) {
            bool tmp_value = tmp_irq.get_bit(i);
            zynq.pl2ps_irq[i].write(tmp_value);
        }
    }

    // Generate negative reset (if needed internally)
    void gen_rst_n(void)
    {
        rst_n.write(!rst.read());
    }

    SC_HAS_PROCESS(zynq7_ps);
    // The combination of SC_HAS_PROCESS and a normal constructor replaces the use of SC_CTOR
    // SC_HAS_PROCESS is only needed if you have SC_THREADs or SC_METHODs in your module! If there are no processes then you don't need SC_HAS_PROCESS.

    /////////////////
    // Constructor //
    /////////////////
    zynq7_ps(sc_module_name name) :
        // Initialize generics
        ////////////////////////
        // QEMU


        // Modules
        ////////////

        // Set the path to the socket from the generic parameter
//#ifdef __QEMU_PATH_TO_SOCKET__
//        zynq("zynq", __QEMU_PATH_TO_SOCKET__),
//#else
        zynq("zynq", QEMU_PATH_TO_SOCKET_G.c_str()),
//#endif

        // Signals
        ////////////
        rst("rst"),
        fclk_clk0           ("fclk_clk0"),
        fclk_clk1           ("fclk_clk1"),
        fclk_clk2           ("fclk_clk2"),
        fclk_clk3           ("fclk_clk3"),
        
        m_axi_gp0_aclk      ("m_axi_gp0_aclk"),     
        m_axi_gp0_aresetn   ("m_axi_gp0_aresetn"),          
        m_axi_gp0_awvalid   ("m_axi_gp0_awvalid"),      
        m_axi_gp0_awready   ("m_axi_gp0_awready"),      
        m_axi_gp0_awaddr    ("m_axi_gp0_awaddr"),       
        m_axi_gp0_awprot    ("m_axi_gp0_awprot"),       
        m_axi_gp0_awqos     ("m_axi_gp0_awqos"),        
        m_axi_gp0_awcache   ("m_axi_gp0_awcache"),      
        m_axi_gp0_awburst   ("m_axi_gp0_awburst"),      
        m_axi_gp0_awsize    ("m_axi_gp0_awsize"),       
        m_axi_gp0_awlen     ("m_axi_gp0_awlen"),        
        m_axi_gp0_awid      ("m_axi_gp0_awid"),     
        m_axi_gp0_awlock    ("m_axi_gp0_awlock"),
        m_axi_gp0_wvalid    ("m_axi_gp0_wvalid"),
        m_axi_gp0_wready    ("m_axi_gp0_wready"),
        m_axi_gp0_wdata     ("m_axi_gp0_wdata"),
        m_axi_gp0_wstrb     ("m_axi_gp0_wstrb"),
        m_axi_gp0_wlast     ("m_axi_gp0_wlast"),
        m_axi_gp0_bvalid    ("m_axi_gp0_bvalid"),
        m_axi_gp0_bready    ("m_axi_gp0_bready"),
        m_axi_gp0_bresp     ("m_axi_gp0_bresp"),
        m_axi_gp0_bid       ("m_axi_gp0_bid"),
        m_axi_gp0_arvalid   ("m_axi_gp0_arvalid"),
        m_axi_gp0_arready   ("m_axi_gp0_arready"),
        m_axi_gp0_araddr    ("m_axi_gp0_araddr"),
        m_axi_gp0_arprot    ("m_axi_gp0_arprot"),
        m_axi_gp0_arqos     ("m_axi_gp0_arqos"),
        m_axi_gp0_arcache   ("m_axi_gp0_arcache"),
        m_axi_gp0_arburst   ("m_axi_gp0_arburst"),
        m_axi_gp0_arsize    ("m_axi_gp0_arsize"),
        m_axi_gp0_arlen     ("m_axi_gp0_arlen"),
        m_axi_gp0_arid      ("m_axi_gp0_arid"),
        m_axi_gp0_arlock    ("m_axi_gp0_arlock"),
        m_axi_gp0_rvalid    ("m_axi_gp0_rvalid"),
        m_axi_gp0_rready    ("m_axi_gp0_rready"),
        m_axi_gp0_rdata     ("m_axi_gp0_rdata"),
        m_axi_gp0_rresp     ("m_axi_gp0_rresp"),
        m_axi_gp0_rid       ("m_axi_gp0_rid"),
        m_axi_gp0_rlast     ("m_axi_gp0_rlast"),
        m_axi_gp0_arregion  ("m_axi_gp0_arregion"),
        m_axi_gp0_awregion  ("m_axi_gp0_awregion"),
        
        m_axi_gp1_aclk      ("m_axi_gp1_aclk"),     
        m_axi_gp1_aresetn   ("m_axi_gp1_aresetn"),          
        m_axi_gp1_awvalid   ("m_axi_gp1_awvalid"),      
        m_axi_gp1_awready   ("m_axi_gp1_awready"),      
        m_axi_gp1_awaddr    ("m_axi_gp1_awaddr"),       
        m_axi_gp1_awprot    ("m_axi_gp1_awprot"),       
        m_axi_gp1_awqos     ("m_axi_gp1_awqos"),        
        m_axi_gp1_awcache   ("m_axi_gp1_awcache"),      
        m_axi_gp1_awburst   ("m_axi_gp1_awburst"),      
        m_axi_gp1_awsize    ("m_axi_gp1_awsize"),       
        m_axi_gp1_awlen     ("m_axi_gp1_awlen"),        
        m_axi_gp1_awid      ("m_axi_gp1_awid"),     
        m_axi_gp1_awlock    ("m_axi_gp1_awlock"),
        m_axi_gp1_wvalid    ("m_axi_gp1_wvalid"),
        m_axi_gp1_wready    ("m_axi_gp1_wready"),
        m_axi_gp1_wdata     ("m_axi_gp1_wdata"),
        m_axi_gp1_wstrb     ("m_axi_gp1_wstrb"),
        m_axi_gp1_wlast     ("m_axi_gp1_wlast"),
        m_axi_gp1_bvalid    ("m_axi_gp1_bvalid"),
        m_axi_gp1_bready    ("m_axi_gp1_bready"),
        m_axi_gp1_bresp     ("m_axi_gp1_bresp"),
        m_axi_gp1_bid       ("m_axi_gp1_bid"),
        m_axi_gp1_arvalid   ("m_axi_gp1_arvalid"),
        m_axi_gp1_arready   ("m_axi_gp1_arready"),
        m_axi_gp1_araddr    ("m_axi_gp1_araddr"),
        m_axi_gp1_arprot    ("m_axi_gp1_arprot"),
        m_axi_gp1_arqos     ("m_axi_gp1_arqos"),
        m_axi_gp1_arcache   ("m_axi_gp1_arcache"),
        m_axi_gp1_arburst   ("m_axi_gp1_arburst"),
        m_axi_gp1_arsize    ("m_axi_gp1_arsize"),
        m_axi_gp1_arlen     ("m_axi_gp1_arlen"),
        m_axi_gp1_arid      ("m_axi_gp1_arid"),
        m_axi_gp1_arlock    ("m_axi_gp1_arlock"),
        m_axi_gp1_rvalid    ("m_axi_gp1_rvalid"),
        m_axi_gp1_rready    ("m_axi_gp1_rready"),
        m_axi_gp1_rdata     ("m_axi_gp1_rdata"),
        m_axi_gp1_rresp     ("m_axi_gp1_rresp"),
        m_axi_gp1_rid       ("m_axi_gp1_rid"),
        m_axi_gp1_rlast     ("m_axi_gp1_rlast"),
        m_axi_gp1_arregion  ("m_axi_gp1_arregion"),
        m_axi_gp1_awregion  ("m_axi_gp1_awregion"),     
                
        s_axi_gp0_aclk      ("s_axi_gp0_aclk"),     
        s_axi_gp0_aresetn   ("s_axi_gp0_aresetn"),          
        s_axi_gp0_awvalid   ("s_axi_gp0_awvalid"),      
        s_axi_gp0_awready   ("s_axi_gp0_awready"),      
        s_axi_gp0_awaddr    ("s_axi_gp0_awaddr"),       
        s_axi_gp0_awprot    ("s_axi_gp0_awprot"),       
        s_axi_gp0_awregion  ("s_axi_gp0_awregion"),     
        s_axi_gp0_awqos     ("s_axi_gp0_awqos"),        
        s_axi_gp0_awcache   ("s_axi_gp0_awcache"),      
        s_axi_gp0_awburst   ("s_axi_gp0_awburst"),      
        s_axi_gp0_awsize    ("s_axi_gp0_awsize"),       
        s_axi_gp0_awlen     ("s_axi_gp0_awlen"),            
        s_axi_gp0_awid      ("s_axi_gp0_awid"),     
        s_axi_gp0_awlock    ("s_axi_gp0_awlock"),       
        s_axi_gp0_wvalid    ("s_axi_gp0_wvalid"),       
        s_axi_gp0_wready    ("s_axi_gp0_wready"),       
        s_axi_gp0_wdata     ("s_axi_gp0_wdata"),        
        s_axi_gp0_wstrb     ("s_axi_gp0_wstrb"),    
        s_axi_gp0_wlast     ("s_axi_gp0_wlast"),
        s_axi_gp0_bvalid    ("s_axi_gp0_bvalid"),       
        s_axi_gp0_bready    ("s_axi_gp0_bready"),       
        s_axi_gp0_bresp     ("s_axi_gp0_bresp"),        
        s_axi_gp0_bid       ("s_axi_gp0_bid"), 
        s_axi_gp0_arvalid   ("s_axi_gp0_arvalid"), 
        s_axi_gp0_arready   ("s_axi_gp0_arready"), 
        s_axi_gp0_araddr    ("s_axi_gp0_araddr"), 
        s_axi_gp0_arprot    ("s_axi_gp0_arprot"), 
        s_axi_gp0_arregion  ("s_axi_gp0_arregion"), 
        s_axi_gp0_arqos     ("s_axi_gp0_arqos"), 
        s_axi_gp0_arcache   ("s_axi_gp0_arcache"), 
        s_axi_gp0_arburst   ("s_axi_gp0_arburst"), 
        s_axi_gp0_arsize    ("s_axi_gp0_arsize"), 
        s_axi_gp0_arlen     ("s_axi_gp0_arlen"), 
        s_axi_gp0_arid      ("s_axi_gp0_arid"), 
        s_axi_gp0_arlock    ("s_axi_gp0_arlock"), 
        s_axi_gp0_rvalid    ("s_axi_gp0_rvalid"),
        s_axi_gp0_rready    ("s_axi_gp0_rready"),
        s_axi_gp0_rdata     ("s_axi_gp0_rdata"),
        s_axi_gp0_rresp     ("s_axi_gp0_rresp"),
        s_axi_gp0_rid       ("s_axi_gp0_rid"),
        s_axi_gp0_rlast     ("s_axi_gp0_rlast"),
 
        s_axi_gp1_aclk      ("s_axi_gp1_aclk"),     
        s_axi_gp1_aresetn   ("s_axi_gp1_aresetn"),          
        s_axi_gp1_awvalid   ("s_axi_gp1_awvalid"),      
        s_axi_gp1_awready   ("s_axi_gp1_awready"),      
        s_axi_gp1_awaddr    ("s_axi_gp1_awaddr"),       
        s_axi_gp1_awprot    ("s_axi_gp1_awprot"),       
        s_axi_gp1_awregion  ("s_axi_gp1_awregion"),     
        s_axi_gp1_awqos     ("s_axi_gp1_awqos"),        
        s_axi_gp1_awcache   ("s_axi_gp1_awcache"),      
        s_axi_gp1_awburst   ("s_axi_gp1_awburst"),      
        s_axi_gp1_awsize    ("s_axi_gp1_awsize"),       
        s_axi_gp1_awlen     ("s_axi_gp1_awlen"),            
        s_axi_gp1_awid      ("s_axi_gp1_awid"),     
        s_axi_gp1_awlock    ("s_axi_gp1_awlock"),       
        s_axi_gp1_wvalid    ("s_axi_gp1_wvalid"),       
        s_axi_gp1_wready    ("s_axi_gp1_wready"),       
        s_axi_gp1_wdata     ("s_axi_gp1_wdata"),        
        s_axi_gp1_wstrb     ("s_axi_gp1_wstrb"),    
        s_axi_gp1_wlast     ("s_axi_gp1_wlast"),
        s_axi_gp1_bvalid    ("s_axi_gp1_bvalid"),       
        s_axi_gp1_bready    ("s_axi_gp1_bready"),       
        s_axi_gp1_bresp     ("s_axi_gp1_bresp"),        
        s_axi_gp1_bid       ("s_axi_gp1_bid"), 
        s_axi_gp1_arvalid   ("s_axi_gp1_arvalid"), 
        s_axi_gp1_arready   ("s_axi_gp1_arready"), 
        s_axi_gp1_araddr    ("s_axi_gp1_araddr"), 
        s_axi_gp1_arprot    ("s_axi_gp1_arprot"), 
        s_axi_gp1_arregion  ("s_axi_gp1_arregion"), 
        s_axi_gp1_arqos     ("s_axi_gp1_arqos"), 
        s_axi_gp1_arcache   ("s_axi_gp1_arcache"), 
        s_axi_gp1_arburst   ("s_axi_gp1_arburst"), 
        s_axi_gp1_arsize    ("s_axi_gp1_arsize"), 
        s_axi_gp1_arlen     ("s_axi_gp1_arlen"), 
        s_axi_gp1_arid      ("s_axi_gp1_arid"), 
        s_axi_gp1_arlock    ("s_axi_gp1_arlock"), 
        s_axi_gp1_rvalid    ("s_axi_gp1_rvalid"),
        s_axi_gp1_rready    ("s_axi_gp1_rready"),
        s_axi_gp1_rdata     ("s_axi_gp1_rdata"),
        s_axi_gp1_rresp     ("s_axi_gp1_rresp"),
        s_axi_gp1_rid       ("s_axi_gp1_rid"),
        s_axi_gp1_rlast     ("s_axi_gp1_rlast"),
 
 
        s_axi_hp0_aclk      ("s_axi_hp0_aclk"),
        s_axi_hp0_aresetn   ("s_axi_hp0_aresetn"),
        s_axi_hp0_awvalid   ("s_axi_hp0_awvalid"),
        s_axi_hp0_awready   ("s_axi_hp0_awready"),
        s_axi_hp0_awaddr    ("s_axi_hp0_awaddr"),
        s_axi_hp0_awprot    ("s_axi_hp0_awprot"),
        s_axi_hp0_awregion  ("s_axi_hp0_awregion"),
        s_axi_hp0_awqos     ("s_axi_hp0_awqos"),
        s_axi_hp0_awcache   ("s_axi_hp0_awcache"),
        s_axi_hp0_awburst   ("s_axi_hp0_awburst"),
        s_axi_hp0_awsize    ("s_axi_hp0_awsize"),
        s_axi_hp0_awlen     ("s_axi_hp0_awlen"),
        s_axi_hp0_awid      ("s_axi_hp0_awid"),
        s_axi_hp0_awlock    ("s_axi_hp0_awlock"),
        s_axi_hp0_wvalid    ("s_axi_hp0_wvalid"),
        s_axi_hp0_wready    ("s_axi_hp0_wready"),
        s_axi_hp0_wdata     ("s_axi_hp0_wdata"),
        s_axi_hp0_wstrb     ("s_axi_hp0_wstrb"),
        s_axi_hp0_wlast     ("s_axi_hp0_wlast"),
        s_axi_hp0_bvalid    ("s_axi_hp0_bvalid"),
        s_axi_hp0_bready    ("s_axi_hp0_bready"),
        s_axi_hp0_bresp     ("s_axi_hp0_bresp"),
        s_axi_hp0_bid       ("s_axi_hp0_bid"),
        s_axi_hp0_arvalid   ("s_axi_hp0_arvalid"),
        s_axi_hp0_arready   ("s_axi_hp0_arready"),
        s_axi_hp0_araddr    ("s_axi_hp0_araddr"),
        s_axi_hp0_arprot    ("s_axi_hp0_arprot"),
        s_axi_hp0_arregion  ("s_axi_hp0_arregion"),
        s_axi_hp0_arqos     ("s_axi_hp0_arqos"),
        s_axi_hp0_arcache   ("s_axi_hp0_arcache"),
        s_axi_hp0_arburst   ("s_axi_hp0_arburst"),
        s_axi_hp0_arsize    ("s_axi_hp0_arsize"),
        s_axi_hp0_arlen     ("s_axi_hp0_arlen"),
        s_axi_hp0_arid      ("s_axi_hp0_arid"),
        s_axi_hp0_arlock    ("s_axi_hp0_arlock"),
        s_axi_hp0_rvalid    ("s_axi_hp0_rvalid"),
        s_axi_hp0_rready    ("s_axi_hp0_rready"),
        s_axi_hp0_rdata     ("s_axi_hp0_rdata"),
        s_axi_hp0_rresp     ("s_axi_hp0_rresp"),
        s_axi_hp0_rid       ("s_axi_hp0_rid"),
        s_axi_hp0_rlast     ("s_axi_hp0_rlast"), 
         
        s_axi_hp1_aclk      ("s_axi_hp1_aclk"),
        s_axi_hp1_aresetn   ("s_axi_hp1_aresetn"),
        s_axi_hp1_awvalid   ("s_axi_hp1_awvalid"),
        s_axi_hp1_awready   ("s_axi_hp1_awready"),
        s_axi_hp1_awaddr    ("s_axi_hp1_awaddr"),
        s_axi_hp1_awprot    ("s_axi_hp1_awprot"),
        s_axi_hp1_awregion  ("s_axi_hp1_awregion"),
        s_axi_hp1_awqos     ("s_axi_hp1_awqos"),
        s_axi_hp1_awcache   ("s_axi_hp1_awcache"),
        s_axi_hp1_awburst   ("s_axi_hp1_awburst"),
        s_axi_hp1_awsize    ("s_axi_hp1_awsize"),
        s_axi_hp1_awlen     ("s_axi_hp1_awlen"),
        s_axi_hp1_awid      ("s_axi_hp1_awid"),
        s_axi_hp1_awlock    ("s_axi_hp1_awlock"),
        s_axi_hp1_wvalid    ("s_axi_hp1_wvalid"),
        s_axi_hp1_wready    ("s_axi_hp1_wready"),
        s_axi_hp1_wdata     ("s_axi_hp1_wdata"),
        s_axi_hp1_wstrb     ("s_axi_hp1_wstrb"),
        s_axi_hp1_wlast     ("s_axi_hp1_wlast"),
        s_axi_hp1_bvalid    ("s_axi_hp1_bvalid"),
        s_axi_hp1_bready    ("s_axi_hp1_bready"),
        s_axi_hp1_bresp     ("s_axi_hp1_bresp"),
        s_axi_hp1_bid       ("s_axi_hp1_bid"),
        s_axi_hp1_arvalid   ("s_axi_hp1_arvalid"),
        s_axi_hp1_arready   ("s_axi_hp1_arready"),
        s_axi_hp1_araddr    ("s_axi_hp1_araddr"),
        s_axi_hp1_arprot    ("s_axi_hp1_arprot"),
        s_axi_hp1_arregion  ("s_axi_hp1_arregion"),
        s_axi_hp1_arqos     ("s_axi_hp1_arqos"),
        s_axi_hp1_arcache   ("s_axi_hp1_arcache"),
        s_axi_hp1_arburst   ("s_axi_hp1_arburst"),
        s_axi_hp1_arsize    ("s_axi_hp1_arsize"),
        s_axi_hp1_arlen     ("s_axi_hp1_arlen"),
        s_axi_hp1_arid      ("s_axi_hp1_arid"),
        s_axi_hp1_arlock    ("s_axi_hp1_arlock"),
        s_axi_hp1_rvalid    ("s_axi_hp1_rvalid"),
        s_axi_hp1_rready    ("s_axi_hp1_rready"),
        s_axi_hp1_rdata     ("s_axi_hp1_rdata"),
        s_axi_hp1_rresp     ("s_axi_hp1_rresp"),
        s_axi_hp1_rid       ("s_axi_hp1_rid"),
        s_axi_hp1_rlast     ("s_axi_hp1_rlast"), 
 
        s_axi_hp2_aclk      ("s_axi_hp2_aclk"),
        s_axi_hp2_aresetn   ("s_axi_hp2_aresetn"),
        s_axi_hp2_awvalid   ("s_axi_hp2_awvalid"),
        s_axi_hp2_awready   ("s_axi_hp2_awready"),
        s_axi_hp2_awaddr    ("s_axi_hp2_awaddr"),
        s_axi_hp2_awprot    ("s_axi_hp2_awprot"),
        s_axi_hp2_awregion  ("s_axi_hp2_awregion"),
        s_axi_hp2_awqos     ("s_axi_hp2_awqos"),
        s_axi_hp2_awcache   ("s_axi_hp2_awcache"),
        s_axi_hp2_awburst   ("s_axi_hp2_awburst"),
        s_axi_hp2_awsize    ("s_axi_hp2_awsize"),
        s_axi_hp2_awlen     ("s_axi_hp2_awlen"),
        s_axi_hp2_awid      ("s_axi_hp2_awid"),
        s_axi_hp2_awlock    ("s_axi_hp2_awlock"),
        s_axi_hp2_wvalid    ("s_axi_hp2_wvalid"),
        s_axi_hp2_wready    ("s_axi_hp2_wready"),
        s_axi_hp2_wdata     ("s_axi_hp2_wdata"),
        s_axi_hp2_wstrb     ("s_axi_hp2_wstrb"),
        s_axi_hp2_wlast     ("s_axi_hp2_wlast"),
        s_axi_hp2_bvalid    ("s_axi_hp2_bvalid"),
        s_axi_hp2_bready    ("s_axi_hp2_bready"),
        s_axi_hp2_bresp     ("s_axi_hp2_bresp"),
        s_axi_hp2_bid       ("s_axi_hp2_bid"),
        s_axi_hp2_arvalid   ("s_axi_hp2_arvalid"),
        s_axi_hp2_arready   ("s_axi_hp2_arready"),
        s_axi_hp2_araddr    ("s_axi_hp2_araddr"),
        s_axi_hp2_arprot    ("s_axi_hp2_arprot"),
        s_axi_hp2_arregion  ("s_axi_hp2_arregion"),
        s_axi_hp2_arqos     ("s_axi_hp2_arqos"),
        s_axi_hp2_arcache   ("s_axi_hp2_arcache"),
        s_axi_hp2_arburst   ("s_axi_hp2_arburst"),
        s_axi_hp2_arsize    ("s_axi_hp2_arsize"),
        s_axi_hp2_arlen     ("s_axi_hp2_arlen"),
        s_axi_hp2_arid      ("s_axi_hp2_arid"),
        s_axi_hp2_arlock    ("s_axi_hp2_arlock"),
        s_axi_hp2_rvalid    ("s_axi_hp2_rvalid"),
        s_axi_hp2_rready    ("s_axi_hp2_rready"),
        s_axi_hp2_rdata     ("s_axi_hp2_rdata"),
        s_axi_hp2_rresp     ("s_axi_hp2_rresp"),
        s_axi_hp2_rid       ("s_axi_hp2_rid"),
        s_axi_hp2_rlast     ("s_axi_hp2_rlast"), 
 
 
  
        s_axi_hp3_aclk      ("s_axi_hp3_aclk"),
        s_axi_hp3_aresetn   ("s_axi_hp3_aresetn"),
        s_axi_hp3_awvalid   ("s_axi_hp3_awvalid"),
        s_axi_hp3_awready   ("s_axi_hp3_awready"),
        s_axi_hp3_awaddr    ("s_axi_hp3_awaddr"),
        s_axi_hp3_awprot    ("s_axi_hp3_awprot"),
        s_axi_hp3_awregion  ("s_axi_hp3_awregion"),
        s_axi_hp3_awqos     ("s_axi_hp3_awqos"),
        s_axi_hp3_awcache   ("s_axi_hp3_awcache"),
        s_axi_hp3_awburst   ("s_axi_hp3_awburst"),
        s_axi_hp3_awsize    ("s_axi_hp3_awsize"),
        s_axi_hp3_awlen     ("s_axi_hp3_awlen"),
        s_axi_hp3_awid      ("s_axi_hp3_awid"),
        s_axi_hp3_awlock    ("s_axi_hp3_awlock"),
        s_axi_hp3_wvalid    ("s_axi_hp3_wvalid"),
        s_axi_hp3_wready    ("s_axi_hp3_wready"),
        s_axi_hp3_wdata     ("s_axi_hp3_wdata"),
        s_axi_hp3_wstrb     ("s_axi_hp3_wstrb"),
        s_axi_hp3_wlast     ("s_axi_hp3_wlast"),
        s_axi_hp3_bvalid    ("s_axi_hp3_bvalid"),
        s_axi_hp3_bready    ("s_axi_hp3_bready"),
        s_axi_hp3_bresp     ("s_axi_hp3_bresp"),
        s_axi_hp3_bid       ("s_axi_hp3_bid"),
        s_axi_hp3_arvalid   ("s_axi_hp3_arvalid"),
        s_axi_hp3_arready   ("s_axi_hp3_arready"),
        s_axi_hp3_araddr    ("s_axi_hp3_araddr"),
        s_axi_hp3_arprot    ("s_axi_hp3_arprot"),
        s_axi_hp3_arregion  ("s_axi_hp3_arregion"),
        s_axi_hp3_arqos     ("s_axi_hp3_arqos"),
        s_axi_hp3_arcache   ("s_axi_hp3_arcache"),
        s_axi_hp3_arburst   ("s_axi_hp3_arburst"),
        s_axi_hp3_arsize    ("s_axi_hp3_arsize"),
        s_axi_hp3_arlen     ("s_axi_hp3_arlen"),
        s_axi_hp3_arid      ("s_axi_hp3_arid"),
        s_axi_hp3_arlock    ("s_axi_hp3_arlock"),
        s_axi_hp3_rvalid    ("s_axi_hp3_rvalid"),
        s_axi_hp3_rready    ("s_axi_hp3_rready"),
        s_axi_hp3_rdata     ("s_axi_hp3_rdata"),
        s_axi_hp3_rresp     ("s_axi_hp3_rresp"),
        s_axi_hp3_rid       ("s_axi_hp3_rid"),
        s_axi_hp3_rlast     ("s_axi_hp3_rlast"), 

        irq_f2p             ("irq_f2p"), 
        rst_o               ("rst_o") 
    {
        // QEMU related
        /////////////////
        cout << "QEMU connection path is : " << QEMU_PATH_TO_SOCKET_G << endl;
        cout << "QEMU_SYNC_QUANTUM_G : " << QEMU_SYNC_QUANTUM_G << endl;

        // Set the global sim time quantum from the generic parameter
        m_qk.set_global_quantum(sc_time((double) QEMU_SYNC_QUANTUM_G, SC_NS));

        // Reset
        //////////
        SC_METHOD(gen_rst_n);
        sensitive << rst;

        zynq.rst(rst);
        
        
        ////////////////////
        // Generics
        ////////////////////
        // generic registration the RivieraPRO style
        // to get a generic see section above with the function sc_get_param(<GenericName>, <GenericVariable>)
        register_string_parameter("QEMU_PATH_TO_SOCKET_G");
        register_int_parameter("QEMU_SYNC_QUANTUM_G");
        register_int_parameter("FCLK_CLK0_PERIOD_IN_NS_G");
        register_int_parameter("FCLK_CLK1_PERIOD_IN_NS_G");
        register_int_parameter("FCLK_CLK2_PERIOD_IN_NS_G");
        register_int_parameter("FCLK_CLK3_PERIOD_IN_NS_G");
        register_int_parameter("M_AXI_GP0_ENABLE_G");
        register_int_parameter("M_AXI_GP1_ENABLE_G");
        register_int_parameter("S_AXI_GP0_ENABLE_G");
        register_int_parameter("S_AXI_GP1_ENABLE_G");
        register_int_parameter("S_AXI_HP0_ENABLE_G");
        register_int_parameter("S_AXI_HP1_ENABLE_G");
        register_int_parameter("S_AXI_HP2_ENABLE_G");
        register_int_parameter("S_AXI_HP3_ENABLE_G");

        ////////////////////
        // AXI Interfaces //
        ////////////////////

        // M AXI GP0
        //////////////
      

        ////////////
        // Clocks //
        ////////////

        // FCLK_CLK0
        assert(FCLK_CLK0_PERIOD_IN_NS_G > 0); // This clock is mandatory (arbitrary choice)
        fclk_clk0_gen = new sc_clock("fclk_clk0_gen", sc_time(FCLK_CLK0_PERIOD_IN_NS_G, SC_NS));
        SC_METHOD(fclk_clk0_assign);
        sensitive << fclk_clk0_gen->signal();
        dont_initialize();




        //////////
        // IRQs //
        //////////

        SC_METHOD(assign_irq);
        sensitive << irq_f2p;
        dont_initialize();

        /* Tie off any remaining unconnected signals.  */
        
        SC_THREAD(pull_reset);
    }

private:
    tlm_utils::tlm_quantumkeeper m_qk;
};
