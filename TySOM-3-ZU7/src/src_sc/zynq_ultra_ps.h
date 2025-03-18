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
//#include "xilinx-zynq.h"
#include "xilinx-zynqmp.h"

// Xilinx Bridges (BFMs)
// https://github.com/Xilinx/libsystemctlm-soc in tlm-bridges
#include "tlm2axi-bridge.h"
#include "axi2tlm-bridge.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

SC_MODULE(zynqmp_ps)
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
        int QEMU_SYNC_QUANTUM_G = 1000000;
    #endif

    int FCLK_CLK0_PERIOD_IN_NS_G = 20000;
    int FCLK_CLK1_PERIOD_IN_NS_G = -1;
    int FCLK_CLK2_PERIOD_IN_NS_G = -1;
    int FCLK_CLK3_PERIOD_IN_NS_G = -1;
    //int M_AXI_HPM0_FPD_ENABLE_G = 0;
    //int M_AXI_HPM1_FPD_ENABLE_G = 0;
    //int M_AXI_HPM0_LPD_ENABLE_G = 0;

    int S_AXI_HPC0_FPD_ENABLE_G = 0;
    int S_AXI_HPC1_FPD_ENABLE_G = 0;

    int S_AXI_HP0_FPD_ENABLE_G = 0;
    int S_AXI_HP1_FPD_ENABLE_G = 0;
    int S_AXI_HP2_FPD_ENABLE_G = 0;
    int S_AXI_HP3_FPD_ENABLE_G = 0;

    int S_AXI_LPD_ENABLE_G = 0;
    int S_AXI_ACP_FPD_ENABLE_G = 0;
    int S_AXI_ACE_FPD_ENABLE_G = 0;


#ifdef __M_AXI_HPM0_FPD_ENABLE__
    int M_AXI_HPM0_FPD_ENABLE_G   = __M_AXI_HPM0_FPD_ENABLE__;
#else
    int M_AXI_HPM0_FPD_ENABLE_G   = 0;
#endif

#ifdef __M_AXI_HPM1_FPD_ENABLE__
    int M_AXI_HPM1_FPD_ENABLE_G   = __M_AXI_HPM1_FPD_ENABLE__;
#else
    int M_AXI_HPM1_FPD_ENABLE_G   = 0;
#endif

#ifdef __M_AXI_HPM0_LPD_ENABLE__
    int M_AXI_HPM0_LPD_ENABLE_G   = __M_AXI_HPM0_LPD_ENABLE__;
#else
    int M_AXI_HPM0_LPD_ENABLE_G   = 0;
#endif

    // SystemC generics are implemented as members and can therefore not be used as template parameters
    // Templates require constant (compilation time known values and can not rely on a this->my_constant
    // type of constant apparently
#ifdef __M_AXI_HPM0_FPD_ADDR_WIDTH__
    static const int M_AXI_HPM0_FPD_ADDR_WIDTH_G   = __M_AXI_HPM0_FPD_ADDR_WIDTH__;
#else
    static const int M_AXI_HPM0_FPD_ADDR_WIDTH_G   = 40;
#endif
#ifdef __M_AXI_HPM0_FPD_DATA_WIDTH__
    static const int M_AXI_HPM0_FPD_DATA_WIDTH_G   = __M_AXI_HPM0_FPD_DATA_WIDTH__;
#else
    static const int M_AXI_HPM0_FPD_DATA_WIDTH_G   = 128;
#endif
#ifdef __M_AXI_HPM0_FPD_ID_WIDTH__
    static const int M_AXI_HPM0_FPD_ID_WIDTH_G     = __M_AXI_HPM0_FPD_ID_WIDTH__;
#else
    static const int M_AXI_HPM0_FPD_ID_WIDTH_G     = 16;
#endif
#ifdef __M_AXI_HPM0_FPD_AXLEN_WIDTH__
    static const int M_AXI_HPM0_FPD_AXLEN_WIDTH_G  = __M_AXI_HPM0_FPD_AXLEN_WIDTH__;
#else
    static const int M_AXI_HPM0_FPD_AXLEN_WIDTH_G  = 8;
#endif
#ifdef __M_AXI_HPM0_FPD_AXLOCK_WIDTH__
    static const int M_AXI_HPM0_FPD_AXLOCK_WIDTH_G = __M_AXI_HPM0_FPD_AXLOCK_WIDTH__;
#else
    static const int M_AXI_HPM0_FPD_AXLOCK_WIDTH_G = 1;
#endif

#ifdef __M_AXI_HPM0_FPD_AWUSER_WIDTH__
    static const int M_AXI_HPM0_FPD_AWUSER_WIDTH_G = __M_AXI_HPM0_FPD_AWUSER_WIDTH__;
#else
    static const int M_AXI_HPM0_FPD_AWUSER_WIDTH_G = 16;
#endif
#ifdef __M_AXI_HPM0_FPD_ARUSER_WIDTH__
    static const int M_AXI_HPM0_FPD_ARUSER_WIDTH_G = __M_AXI_HPM0_FPD_ARUSER_WIDTH__;
#else
    static const int M_AXI_HPM0_FPD_ARUSER_WIDTH_G = 16;
#endif


#ifdef __M_AXI_HPM1_FPD_ADDR_WIDTH__
    static const int M_AXI_HPM1_FPD_ADDR_WIDTH_G   = __M_AXI_HPM1_FPD_ADDR_WIDTH__;
#else
    static const int M_AXI_HPM1_FPD_ADDR_WIDTH_G   = 40;
#endif
#ifdef __M_AXI_HPM1_FPD_DATA_WIDTH__
    static const int M_AXI_HPM1_FPD_DATA_WIDTH_G   = __M_AXI_HPM1_FPD_DATA_WIDTH__;
#else
    static const int M_AXI_HPM1_FPD_DATA_WIDTH_G   = 128;
#endif
#ifdef __M_AXI_HPM1_FPD_ID_WIDTH__
    static const int M_AXI_HPM1_FPD_ID_WIDTH_G     = __M_AXI_HPM1_FPD_ID_WIDTH__;
#else
    static const int M_AXI_HPM1_FPD_ID_WIDTH_G     = 16;
#endif
#ifdef __M_AXI_HPM1_FPD_AXLEN_WIDTH__
    static const int M_AXI_HPM1_FPD_AXLEN_WIDTH_G  = __M_AXI_HPM1_FPD_AXLEN_WIDTH__;
#else
    static const int M_AXI_HPM1_FPD_AXLEN_WIDTH_G  = 8;
#endif
#ifdef __M_AXI_HPM1_FPD_AXLOCK_WIDTH__
    static const int M_AXI_HPM1_FPD_AXLOCK_WIDTH_G = __M_AXI_HPM1_FPD_AXLOCK_WIDTH__;
#else
    static const int M_AXI_HPM1_FPD_AXLOCK_WIDTH_G = 0;
#endif
#ifdef __M_AXI_HPM1_FPD_AWUSER_WIDTH__
    static const int M_AXI_HPM1_FPD_AWUSER_WIDTH_G = __M_AXI_HPM1_FPD_AWUSER_WIDTH__;
#else
    static const int M_AXI_HPM1_FPD_AWUSER_WIDTH_G = 16;
#endif
#ifdef __M_AXI_HPM1_FPD_ARUSER_WIDTH__
    static const int M_AXI_HPM1_FPD_ARUSER_WIDTH_G = __M_AXI_HPM1_FPD_ARUSER_WIDTH__;
#else
    static const int M_AXI_HPM1_FPD_ARUSER_WIDTH_G = 16;
#endif


#ifdef __M_AXI_HPM0_LPD_ADDR_WIDTH__
    static const int M_AXI_HPM0_LPD_ADDR_WIDTH_G   = __M_AXI_HPM1_FPD_ADDR_WIDTH__;
#else
    static const int M_AXI_HPM0_LPD_ADDR_WIDTH_G   = 40;
#endif
#ifdef __M_AXI_HPM0_LPD_DATA_WIDTH__
    static const int M_AXI_HPM0_LPD_DATA_WIDTH_G   = __M_AXI_HPM0_LPD_DATA_WIDTH__;
#else
    static const int M_AXI_HPM0_LPD_DATA_WIDTH_G   = 128;
#endif
#ifdef __M_AXI_HPM0_LPD_ID_WIDTH__
    static const int M_AXI_HPM0_LPD_ID_WIDTH_G     = __M_AXI_HPM0_LPD_ID_WIDTH__;
#else
    static const int M_AXI_HPM0_LPD_ID_WIDTH_G     = 16;
#endif
#ifdef __M_AXI_HPM0_LPD_AXLEN_WIDTH__
    static const int M_AXI_HPM0_LPD_AXLEN_WIDTH_G  = __M_AXI_HPM0_LPD_AXLEN_WIDTH__;
#else
    static const int M_AXI_HPM0_LPD_AXLEN_WIDTH_G  = 8;
#endif
#ifdef __M_AXI_HPM0_LPD_AXLOCK_WIDTH__
    static const int M_AXI_HPM0_LPD_AXLOCK_WIDTH_G = __M_AXI_HPM0_LPD_AXLOCK_WIDTH__;
#else
    static const int M_AXI_HPM0_LPD_AXLOCK_WIDTH_G = 1;
#endif
#ifdef __M_AXI_HPM0_LPD_AWUSER_WIDTH__
    static const int M_AXI_HPM0_LPD_AWUSER_WIDTH_G = __M_AXI_HPM0_LPD_AWUSER_WIDTH__;
#else
    static const int M_AXI_HPM0_LPD_AWUSER_WIDTH_G = 16;
#endif
#ifdef __M_AXI_HPM0_LPD_ARUSER_WIDTH__
    static const int M_AXI_HPM0_LPD_ARUSER_WIDTH_G = __M_AXI_HPM0_LPD_ARUSER_WIDTH__;
#else
    static const int M_AXI_HPM0_LPD_ARUSER_WIDTH_G = 16;
#endif


#ifdef __S_AXI_HP0_FPD_ADDR_WIDTH__
    static const int S_AXI_HP0_FPD_ADDR_WIDTH_G   = __S_AXI_HP0_FPD_ADDR_WIDTH__;
#else
    static const int S_AXI_HP0_FPD_ADDR_WIDTH_G   = 49;
#endif
#ifdef __S_AXI_HP0_FPD_DATA_WIDTH__
    static const int S_AXI_HP0_FPD_DATA_WIDTH_G   = __S_AXI_HP0_FPD_DATA_WIDTH__;
#else
    static const int S_AXI_HP0_FPD_DATA_WIDTH_G   = 128;
#endif
#ifdef __S_AXI_HP0_FPD_ID_WIDTH__
    static const int S_AXI_HP0_FPD_ID_WIDTH_G     = __S_AXI_HP0_FPD_ID_WIDTH__;
#else
    static const int S_AXI_HP0_FPD_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_HP0_FPD_AXLEN_WIDTH__
    static const int S_AXI_HP0_FPD_AXLEN_WIDTH_G  = __S_AXI_HP0_FPD_AXLEN_WIDTH__;
#else
    static const int S_AXI_HP0_FPD_AXLEN_WIDTH_G  = 8;
#endif
#ifdef __S_AXI_HP0_FPD_AXLOCK_WIDTH__
    static const int S_AXI_HP0_FPD_AXLOCK_WIDTH_G = __S_AXI_HP0_FPD_AXLOCK_WIDTH__;
#else
    static const int S_AXI_HP0_FPD_AXLOCK_WIDTH_G = 1;
#endif
#ifdef __S_AXI_HP0_FPD_AWUSER_WIDTH__
    static const int S_AXI_HP0_FPD_AWUSER_WIDTH_G = __S_AXI_HP0_FPD_AWUSER_WIDTH__;
#else
    static const int S_AXI_HP0_FPD_AWUSER_WIDTH_G = 1;
#endif
#ifdef __S_AXI_HP0_FPD_ARUSER_WIDTH__
    static const int S_AXI_HP0_FPD_ARUSER_WIDTH_G = __S_AXI_HP0_FPD_ARUSER_WIDTH__;
#else
    static const int S_AXI_HP0_FPD_ARUSER_WIDTH_G = 1;
#endif


#ifdef __S_AXI_HP1_FPD_ADDR_WIDTH__
    static const int S_AXI_HP1_FPD_ADDR_WIDTH_G   = __S_AXI_HP1_FPD_ADDR_WIDTH__;
#else
    static const int S_AXI_HP1_FPD_ADDR_WIDTH_G   = 49;
#endif
#ifdef __S_AXI_HP1_FPD_DATA_WIDTH__
    static const int S_AXI_HP1_FPD_DATA_WIDTH_G   = __S_AXI_HP1_FPD_DATA_WIDTH__;
#else
    static const int S_AXI_HP1_FPD_DATA_WIDTH_G   = 128;
#endif
#ifdef __S_AXI_HP1_FPD_ID_WIDTH__
    static const int S_AXI_HP1_FPD_ID_WIDTH_G     = __S_AXI_HP1_FPD_ID_WIDTH__;
#else
    static const int S_AXI_HP1_FPD_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_HP1_FPD_AXLEN_WIDTH__
    static const int S_AXI_HP1_FPD_AXLEN_WIDTH_G  = __S_AXI_HP1_FPD_AXLEN_WIDTH__;
#else
    static const int S_AXI_HP1_FPD_AXLEN_WIDTH_G  = 8;
#endif
#ifdef __S_AXI_HP1_FPD_AXLOCK_WIDTH__
    static const int S_AXI_HP1_FPD_AXLOCK_WIDTH_G = __S_AXI_HP1_FPD_AXLOCK_WIDTH__;
#else
    static const int S_AXI_HP1_FPD_AXLOCK_WIDTH_G = 1;
#endif
#ifdef __S_AXI_HP1_FPD_AWUSER_WIDTH__
    static const int S_AXI_HP1_FPD_AWUSER_WIDTH_G = __S_AXI_HP1_FPD_AWUSER_WIDTH__;
#else
    static const int S_AXI_HP1_FPD_AWUSER_WIDTH_G = 1;
#endif
#ifdef __S_AXI_HP1_FPD_ARUSER_WIDTH__
    static const int S_AXI_HP1_FPD_ARUSER_WIDTH_G = __S_AXI_HP1_FPD_ARUSER_WIDTH__;
#else
    static const int S_AXI_HP1_FPD_ARUSER_WIDTH_G = 1;
#endif



#ifdef __S_AXI_HP2_FPD_ADDR_WIDTH__
    static const int S_AXI_HP2_FPD_ADDR_WIDTH_G   = __S_AXI_HP2_FPD_ADDR_WIDTH__;
#else
    static const int S_AXI_HP2_FPD_ADDR_WIDTH_G   = 49;
#endif
#ifdef __S_AXI_HP2_FPD_DATA_WIDTH__
    static const int S_AXI_HP2_FPD_DATA_WIDTH_G   = __S_AXI_HP2_FPD_DATA_WIDTH__;
#else
    static const int S_AXI_HP2_FPD_DATA_WIDTH_G   = 128;
#endif
#ifdef __S_AXI_HP2_FPD_ID_WIDTH__
    static const int S_AXI_HP2_FPD_ID_WIDTH_G     = __S_AXI_HP2_FPD_ID_WIDTH__;
#else
    static const int S_AXI_HP2_FPD_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_HP2_FPD_AXLEN_WIDTH__
    static const int S_AXI_HP2_FPD_AXLEN_WIDTH_G  = __S_AXI_HP2_FPD_AXLEN_WIDTH__;
#else
    static const int S_AXI_HP2_FPD_AXLEN_WIDTH_G  = 8;
#endif
#ifdef __S_AXI_HP2_FPD_AXLOCK_WIDTH__
    static const int S_AXI_HP2_FPD_AXLOCK_WIDTH_G = __S_AXI_HP2_FPD_AXLOCK_WIDTH__;
#else
    static const int S_AXI_HP2_FPD_AXLOCK_WIDTH_G = 1;
#endif
#ifdef __S_AXI_HP2_FPD_AWUSER_WIDTH__
    static const int S_AXI_HP2_FPD_AWUSER_WIDTH_G = __S_AXI_HP2_FPD_AWUSER_WIDTH__;
#else
    static const int S_AXI_HP2_FPD_AWUSER_WIDTH_G = 1;
#endif
#ifdef __S_AXI_HP2_FPD_ARUSER_WIDTH__
    static const int S_AXI_HP2_FPD_ARUSER_WIDTH_G = __S_AXI_HP2_FPD_ARUSER_WIDTH__;
#else
    static const int S_AXI_HP2_FPD_ARUSER_WIDTH_G = 1;
#endif


#ifdef __S_AXI_HP3_FPD_ADDR_WIDTH__
    static const int S_AXI_HP3_FPD_ADDR_WIDTH_G   = __S_AXI_HP3_FPD_ADDR_WIDTH__;
#else
    static const int S_AXI_HP3_FPD_ADDR_WIDTH_G   = 49;
#endif
#ifdef __S_AXI_HP3_FPD_DATA_WIDTH__
    static const int S_AXI_HP3_FPD_DATA_WIDTH_G   = __S_AXI_HP3_FPD_DATA_WIDTH__;
#else
    static const int S_AXI_HP3_FPD_DATA_WIDTH_G   = 128;
#endif
#ifdef __S_AXI_HP3_FPD_ID_WIDTH__
    static const int S_AXI_HP3_FPD_ID_WIDTH_G     = __S_AXI_HP3_FPD_ID_WIDTH__;
#else
    static const int S_AXI_HP3_FPD_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_HP3_FPD_AXLEN_WIDTH__
    static const int S_AXI_HP3_FPD_AXLEN_WIDTH_G  = __S_AXI_HP3_FPD_AXLEN_WIDTH__;
#else
    static const int S_AXI_HP3_FPD_AXLEN_WIDTH_G  = 8;
#endif
#ifdef __S_AXI_HP3_FPD_AXLOCK_WIDTH__
    static const int S_AXI_HP3_FPD_AXLOCK_WIDTH_G = __S_AXI_HP3_FPD_AXLOCK_WIDTH__;
#else
    static const int S_AXI_HP3_FPD_AXLOCK_WIDTH_G = 1;
#endif
#ifdef __S_AXI_HP3_FPD_AWUSER_WIDTH__
    static const int S_AXI_HP3_FPD_AWUSER_WIDTH_G = __S_AXI_HP3_FPD_AWUSER_WIDTH__;
#else
    static const int S_AXI_HP3_FPD_AWUSER_WIDTH_G = 1;
#endif
#ifdef __S_AXI_HP3_FPD_ARUSER_WIDTH__
    static const int S_AXI_HP3_FPD_ARUSER_WIDTH_G = __S_AXI_HP3_FPD_ARUSER_WIDTH__;
#else
    static const int S_AXI_HP3_FPD_ARUSER_WIDTH_G = 1;
#endif




#ifdef __S_AXI_HPC0_FPD_ADDR_WIDTH__
    static const int S_AXI_HPC0_FPD_ADDR_WIDTH_G   = __S_AXI_HPC0_FPD_ADDR_WIDTH__;
#else
    static const int S_AXI_HPC0_FPD_ADDR_WIDTH_G   = 49;
#endif
#ifdef __S_AXI_HPC0_FPD_DATA_WIDTH__
    static const int S_AXI_HPC0_FPD_DATA_WIDTH_G   = __S_AXI_HPC0_FPD_DATA_WIDTH__;
#else
    static const int S_AXI_HPC0_FPD_DATA_WIDTH_G   = 128;
#endif
#ifdef __S_AXI_HPC0_FPD_ID_WIDTH__
    static const int S_AXI_HPC0_FPD_ID_WIDTH_G     = __S_AXI_HPC0_FPD_ID_WIDTH__;
#else
    static const int S_AXI_HPC0_FPD_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_HPC0_FPD_AXLEN_WIDTH__
    static const int S_AXI_HPC0_FPD_AXLEN_WIDTH_G  = __S_AXI_HPC0_FPD_AXLEN_WIDTH__;
#else
    static const int S_AXI_HPC0_FPD_AXLEN_WIDTH_G  = 8;
#endif
#ifdef __S_AXI_HPC0_FPD_AXLOCK_WIDTH__
    static const int S_AXI_HPC0_FPD_AXLOCK_WIDTH_G = __S_AXI_HPC0_FPD_AXLOCK_WIDTH__;
#else
    static const int S_AXI_HPC0_FPD_AXLOCK_WIDTH_G = 1;
#endif
#ifdef __S_AXI_HPC0_FPD_AWUSER_WIDTH__
    static const int S_AXI_HPC0_FPD_AWUSER_WIDTH_G = __S_AXI_HPC0_FPD_AWUSER_WIDTH__;
#else
    static const int S_AXI_HPC0_FPD_AWUSER_WIDTH_G = 1;
#endif
#ifdef __S_AXI_HPC0_FPD_ARUSER_WIDTH__
    static const int S_AXI_HPC0_FPD_ARUSER_WIDTH_G = __S_AXI_HPC0_FPD_ARUSER_WIDTH__;
#else
    static const int S_AXI_HPC0_FPD_ARUSER_WIDTH_G = 1;
#endif


#ifdef __S_AXI_HPC1_FPD_ADDR_WIDTH__
    static const int S_AXI_HPC1_FPD_ADDR_WIDTH_G   = __S_AXI_HPC1_FPD_ADDR_WIDTH__;
#else
    static const int S_AXI_HPC1_FPD_ADDR_WIDTH_G   = 49;
#endif
#ifdef __S_AXI_HPC1_FPD_DATA_WIDTH__
    static const int S_AXI_HPC1_FPD_DATA_WIDTH_G   = __S_AXI_HPC1_FPD_DATA_WIDTH__;
#else
    static const int S_AXI_HPC1_FPD_DATA_WIDTH_G   = 128;
#endif
#ifdef __S_AXI_HPC1_FPD_ID_WIDTH__
    static const int S_AXI_HPC1_FPD_ID_WIDTH_G     = __S_AXI_HPC1_FPD_ID_WIDTH__;
#else
    static const int S_AXI_HPC1_FPD_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_HPC1_FPD_AXLEN_WIDTH__
    static const int S_AXI_HPC1_FPD_AXLEN_WIDTH_G  = __S_AXI_HPC1_FPD_AXLEN_WIDTH__;
#else
    static const int S_AXI_HPC1_FPD_AXLEN_WIDTH_G  = 8;
#endif
#ifdef __S_AXI_HPC1_FPD_AXLOCK_WIDTH__
    static const int S_AXI_HPC1_FPD_AXLOCK_WIDTH_G = __S_AXI_HPC1_FPD_AXLOCK_WIDTH__;
#else
    static const int S_AXI_HPC1_FPD_AXLOCK_WIDTH_G = 1;
#endif
#ifdef __S_AXI_HPC1_FPD_AWUSER_WIDTH__
    static const int S_AXI_HPC1_FPD_AWUSER_WIDTH_G = __S_AXI_HPC1_FPD_AWUSER_WIDTH__;
#else
    static const int S_AXI_HPC1_FPD_AWUSER_WIDTH_G = 1;
#endif
#ifdef __S_AXI_HPC1_FPD_ARUSER_WIDTH__
    static const int S_AXI_HPC1_FPD_ARUSER_WIDTH_G = __S_AXI_HPC1_FPD_ARUSER_WIDTH__;
#else
    static const int S_AXI_HPC1_FPD_ARUSER_WIDTH_G = 1;
#endif








#ifdef __S_AXI_LPD_ADDR_WIDTH__
    static const int S_AXI_LPD_ADDR_WIDTH_G   = __S_AXI_LPD_ADDR_WIDTH__;
#else
    static const int S_AXI_LPD_ADDR_WIDTH_G   = 49;
#endif
#ifdef __S_AXI_LPD_DATA_WIDTH__
    static const int S_AXI_LPD_DATA_WIDTH_G   = __S_AXI_LPD_DATA_WIDTH__;
#else
    static const int S_AXI_LPD_DATA_WIDTH_G   = 128;
#endif
#ifdef __S_AXI_LPD_ID_WIDTH__
    static const int S_AXI_LPD_ID_WIDTH_G     = __S_AXI_LPD_ID_WIDTH__;
#else
    static const int S_AXI_LPD_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_LPD_AXLEN_WIDTH__
    static const int S_AXI_LPD_AXLEN_WIDTH_G  = __S_AXI_LPD_AXLEN_WIDTH__;
#else
    static const int S_AXI_LPD_AXLEN_WIDTH_G  = 8;
#endif
#ifdef __S_AXI_LPD_AXLOCK_WIDTH__
    static const int S_AXI_LPD_AXLOCK_WIDTH_G = __S_AXI_LPD_AXLOCK_WIDTH__;
#else
    static const int S_AXI_LPD_AXLOCK_WIDTH_G = 1;
#endif
#ifdef __S_AXI_LPD_AWUSER_WIDTH__
    static const int S_AXI_LPD_AWUSER_WIDTH_G = __S_AXI_LPD_AWUSER_WIDTH__;
#else
    static const int S_AXI_LPD_AWUSER_WIDTH_G = 1;
#endif
#ifdef __S_AXI_LPD_ARUSER_WIDTH__
    static const int S_AXI_LPD_ARUSER_WIDTH_G = __S_AXI_LPD_ARUSER_WIDTH__;
#else
    static const int S_AXI_LPD_ARUSER_WIDTH_G = 1;
#endif



#ifdef __S_AXI_ACP_FPD_ADDR_WIDTH__
    static const int S_AXI_ACP_FPD_ADDR_WIDTH_G   = __S_AXI_ACP_FPD_ADDR_WIDTH__;
#else
    static const int S_AXI_ACP_FPD_ADDR_WIDTH_G   = 40;
#endif
#ifdef __S_AXI_ACP_FPD_DATA_WIDTH__
    static const int S_AXI_ACP_FPD_DATA_WIDTH_G   = __S_AXI_ACP_FPD_DATA_WIDTH__;
#else
    static const int S_AXI_ACP_FPD_DATA_WIDTH_G   = 128;
#endif
#ifdef __S_AXI_ACP_FPD_ID_WIDTH__
    static const int S_AXI_ACP_FPD_ID_WIDTH_G     = __S_AXI_ACP_FPD_ID_WIDTH__;
#else
    static const int S_AXI_ACP_FPD_ID_WIDTH_G     = 5;
#endif
#ifdef __S_AXI_ACP_FPD_AXLEN_WIDTH__
    static const int S_AXI_ACP_FPD_AXLEN_WIDTH_G  = __S_AXI_ACP_FPD_AXLEN_WIDTH__;
#else
    static const int S_AXI_ACP_FPD_AXLEN_WIDTH_G  = 8;
#endif
#ifdef __S_AXI_ACP_FPD_AXLOCK_WIDTH__
    static const int S_AXI_ACP_FPD_AXLOCK_WIDTH_G = __S_AXI_ACP_FPD_AXLOCK_WIDTH__;
#else
    static const int S_AXI_ACP_FPD_AXLOCK_WIDTH_G = 1;
#endif
#ifdef __S_AXI_ACP_FPD_AWUSER_WIDTH__
    static const int S_AXI_ACP_FPD_AWUSER_WIDTH_G = __S_AXI_ACP_FPD_AWUSER_WIDTH__;
#else
    static const int S_AXI_ACP_FPD_AWUSER_WIDTH_G = 1;
#endif
#ifdef __S_AXI_ACP_FPD_ARUSER_WIDTH__
    static const int S_AXI_ACP_FPD_ARUSER_WIDTH_G = __S_AXI_ACP_FPD_ARUSER_WIDTH__;
#else
    static const int S_AXI_ACP_FPD_ARUSER_WIDTH_G = 1;
#endif



#ifdef __S_AXI_ACE_FPD_ADDR_WIDTH__
    static const int S_AXI_ACE_FPD_ADDR_WIDTH_G   = __S_AXI_ACE_FPD_ADDR_WIDTH__;
#else
    static const int S_AXI_ACE_FPD_ADDR_WIDTH_G   = 44;
#endif
#ifdef __S_AXI_ACE_FPD_DATA_WIDTH__
    static const int S_AXI_ACE_FPD_DATA_WIDTH_G   = __S_AXI_ACE_FPD_DATA_WIDTH__;
#else
    static const int S_AXI_ACE_FPD_DATA_WIDTH_G   = 128;
#endif
#ifdef __S_AXI_ACE_FPD_ID_WIDTH__
    static const int S_AXI_ACE_FPD_ID_WIDTH_G     = __S_AXI_ACE_FPD_ID_WIDTH__;
#else
    static const int S_AXI_ACE_FPD_ID_WIDTH_G     = 6;
#endif
#ifdef __S_AXI_ACE_FPD_AXLEN_WIDTH__
    static const int S_AXI_ACE_FPD_AXLEN_WIDTH_G  = __S_AXI_ACE_FPD_AXLEN_WIDTH__;
#else
    static const int S_AXI_ACE_FPD_AXLEN_WIDTH_G  = 8;
#endif
#ifdef __S_AXI_ACE_FPD_AXLOCK_WIDTH__
    static const int S_AXI_ACE_FPD_AXLOCK_WIDTH_G = __S_AXI_ACE_FPD_AXLOCK_WIDTH__;
#else
    static const int S_AXI_ACE_FPD_AXLOCK_WIDTH_G = 1;
#endif
#ifdef __S_AXI_ACE_FPD_AWUSER_WIDTH__
    static const int S_AXI_ACE_FPD_AWUSER_WIDTH_G = __S_AXI_ACE_FPD_AWUSER_WIDTH__;
#else
    static const int S_AXI_ACE_FPD_AWUSER_WIDTH_G = 1;
#endif
#ifdef __S_AXI_ACE_FPD_ARUSER_WIDTH__
    static const int S_AXI_ACE_FPD_ARUSER_WIDTH_G = __S_AXI_ACE_FPD_ARUSER_WIDTH__;
#else
    static const int S_AXI_ACE_FPD_ARUSER_WIDTH_G = 1;
#endif





    //--------------------------------------------------------------------------------
    // Module
    //--------------------------------------------------------------------------------
    xilinx_zynqmp zynq;

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
    tlm2axi_bridge<M_AXI_HPM0_FPD_ADDR_WIDTH_G,
                   M_AXI_HPM0_FPD_DATA_WIDTH_G,
                   M_AXI_HPM0_FPD_ID_WIDTH_G,
                   M_AXI_HPM0_FPD_AXLEN_WIDTH_G,
                   M_AXI_HPM0_FPD_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0> *tlm2axi_gp0;

    // Pointer to bridge for M AXI GP1
    tlm2axi_bridge<M_AXI_HPM1_FPD_ADDR_WIDTH_G,
                   M_AXI_HPM1_FPD_DATA_WIDTH_G,
                   M_AXI_HPM1_FPD_ID_WIDTH_G,
                   M_AXI_HPM1_FPD_AXLEN_WIDTH_G,
                   M_AXI_HPM1_FPD_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0> *tlm2axi_gp1;

    // Pointer to bridge for M AXI GP2
    tlm2axi_bridge<M_AXI_HPM0_LPD_ADDR_WIDTH_G,
                   M_AXI_HPM0_LPD_DATA_WIDTH_G,
                   M_AXI_HPM0_LPD_ID_WIDTH_G,
                   M_AXI_HPM0_LPD_AXLEN_WIDTH_G,
                   M_AXI_HPM0_LPD_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0> *tlm2axi_gp2;

    // Pointer to bridge for S AXI HPC0
    axi2tlm_bridge<S_AXI_HPC0_FPD_ADDR_WIDTH_G,
                   S_AXI_HPC0_FPD_DATA_WIDTH_G,
                   S_AXI_HPC0_FPD_ID_WIDTH_G,
                   S_AXI_HPC0_FPD_AXLEN_WIDTH_G,
                   S_AXI_HPC0_FPD_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0> *axi2tlm_gp0;

    // Pointer to bridge for S AXI HPC1
    axi2tlm_bridge<S_AXI_HPC1_FPD_ADDR_WIDTH_G,
                   S_AXI_HPC1_FPD_DATA_WIDTH_G,
                   S_AXI_HPC1_FPD_ID_WIDTH_G,
                   S_AXI_HPC1_FPD_AXLEN_WIDTH_G,
                   S_AXI_HPC1_FPD_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0> *axi2tlm_gp1;

    // Pointer to bridge for S AXI HP0
    axi2tlm_bridge<S_AXI_HP0_FPD_ADDR_WIDTH_G,
                   S_AXI_HP0_FPD_DATA_WIDTH_G,
                   S_AXI_HP0_FPD_ID_WIDTH_G,
                   S_AXI_HP0_FPD_AXLEN_WIDTH_G,
                   S_AXI_HP0_FPD_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0> *axi2tlm_gp2;

    // Pointer to bridge for S AXI HP1
    axi2tlm_bridge<S_AXI_HP1_FPD_ADDR_WIDTH_G,
                   S_AXI_HP1_FPD_DATA_WIDTH_G,
                   S_AXI_HP1_FPD_ID_WIDTH_G,
                   S_AXI_HP1_FPD_AXLEN_WIDTH_G,
                   S_AXI_HP1_FPD_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0> *axi2tlm_gp3;


    // Pointer to bridge for S AXI HP2
    axi2tlm_bridge<S_AXI_HP2_FPD_ADDR_WIDTH_G,
                   S_AXI_HP2_FPD_DATA_WIDTH_G,
                   S_AXI_HP2_FPD_ID_WIDTH_G,
                   S_AXI_HP2_FPD_AXLEN_WIDTH_G,
                   S_AXI_HP2_FPD_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0> *axi2tlm_gp4;

    // Pointer to bridge for S AXI HP4
    axi2tlm_bridge<S_AXI_HP3_FPD_ADDR_WIDTH_G,
                   S_AXI_HP3_FPD_DATA_WIDTH_G,
                   S_AXI_HP3_FPD_ID_WIDTH_G,
                   S_AXI_HP3_FPD_AXLEN_WIDTH_G,
                   S_AXI_HP3_FPD_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0> *axi2tlm_gp5;

    // Pointer to bridge for S AXI LPD
    axi2tlm_bridge<S_AXI_LPD_ADDR_WIDTH_G,
                   S_AXI_LPD_DATA_WIDTH_G,
                   S_AXI_LPD_ID_WIDTH_G,
                   S_AXI_LPD_AXLEN_WIDTH_G,
                   S_AXI_LPD_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0> *axi2tlm_gp6;

    // Pointer to bridge for S AXI ACP FPD
    axi2tlm_bridge<S_AXI_ACP_FPD_ADDR_WIDTH_G,
                   S_AXI_ACP_FPD_DATA_WIDTH_G,
                   S_AXI_ACP_FPD_ID_WIDTH_G,
                   S_AXI_ACP_FPD_AXLEN_WIDTH_G,
                   S_AXI_ACP_FPD_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0> *axi2tlm_acp;

    // Pointer to bridge for S AXI ACE FPD
    axi2tlm_bridge<S_AXI_ACE_FPD_ADDR_WIDTH_G,
                   S_AXI_ACE_FPD_DATA_WIDTH_G,
                   S_AXI_ACE_FPD_ID_WIDTH_G,
                   S_AXI_ACE_FPD_AXLEN_WIDTH_G,
                   S_AXI_ACE_FPD_AXLOCK_WIDTH_G,
                   0,
                   0,
                   0,
                   0,
                   0,
                   1> *axi2tlm_ace;



    // M AXI GP0 Ports
    ////////////////////

    sc_in<bool>                              maxigp0_aclk;
    sc_in<bool>                              maxigp0_aresetn;

    /* Write address channel.  */
    sc_out<bool >                            maxigp0_awvalid;
    sc_in<bool >                             maxigp0_awready;
    sc_out<sc_bv<M_AXI_HPM0_FPD_ADDR_WIDTH_G> >   maxigp0_awaddr;
    sc_out<sc_bv<3> >                        maxigp0_awprot;
    sc_out<sc_bv<4> >                        maxigp0_awqos;
    sc_out<sc_bv<4> >                        maxigp0_awcache;
    sc_out<sc_bv<2> >                        maxigp0_awburst;
    sc_out<sc_bv<3> >                        maxigp0_awsize;
    sc_out<sc_bv<M_AXI_HPM0_FPD_AXLEN_WIDTH_G> >  maxigp0_awlen;
    sc_out<sc_bv<M_AXI_HPM0_FPD_ID_WIDTH_G> >     maxigp0_awid;
//#if M_AXI_HPM0_FPD_AXLOCK_WIDTH_G == 1
    sc_out<bool>                             maxigp0_awlock;
//#else
//  sc_out<sc_bv<M_AXI_HPM0_FPD_AXLOCK_WIDTH_G> > maxigp0_awlock;
//#endif
    // BFM side is shown below : so some modification may be needed for these signals
    // Each time BFM side differs is shown in comments below
    //sc_out<AXISignal(AxLEN_WIDTH) > awlen;
    //sc_out<AXISignal(ID_WIDTH) > awid;
    //sc_out<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_out<sc_bv<M_AXI_HPM0_FPD_ID_WIDTH_G> >     maxigp0_wid;
    //sc_out<AXISignal(ID_WIDTH) > wid;
    sc_out<bool >                            maxigp0_wvalid;
    sc_in<bool >                             maxigp0_wready;
    sc_out<sc_bv<M_AXI_HPM0_FPD_DATA_WIDTH_G> >   maxigp0_wdata;
    sc_out<sc_bv<M_AXI_HPM0_FPD_DATA_WIDTH_G/8> > maxigp0_wstrb;
    sc_out<bool >                            maxigp0_wlast;

    /* Write response channel.  */
    sc_in<bool >                             maxigp0_bvalid;
    sc_out<bool >                            maxigp0_bready;
    sc_in<sc_bv<2> >                         maxigp0_bresp;
    //sc_in<AXISignal(BUSER_WIDTH) > buser; // UNUSED so tied-off
    sc_in<sc_bv<M_AXI_HPM0_FPD_ID_WIDTH_G> >      maxigp0_bid;
    //sc_in<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_out<bool >                            maxigp0_arvalid;
    sc_in<bool >                             maxigp0_arready;
    sc_out<sc_bv<M_AXI_HPM0_FPD_ADDR_WIDTH_G> >   maxigp0_araddr;
    sc_out<sc_bv<3> >                        maxigp0_arprot;
    sc_out<sc_bv<4> >                        maxigp0_arqos;
    sc_out<sc_bv<4> >                        maxigp0_arcache;
    sc_out<sc_bv<2> >                        maxigp0_arburst;
    sc_out<sc_bv<3> >                        maxigp0_arsize;
    sc_out<sc_bv<M_AXI_HPM0_FPD_AXLEN_WIDTH_G> >  maxigp0_arlen;
    sc_out<sc_bv<M_AXI_HPM0_FPD_ID_WIDTH_G> >     maxigp0_arid;
//#if M_AXI_HPM0_FPD_AXLOCK_WIDTH_G == 1
    sc_out<bool>                             maxigp0_arlock;
//#else
//  sc_out<sc_bv<M_AXI_HPM0_FPD_AXLOCK_WIDTH_G> > maxigp0_arlock;
//#endif
    //sc_out<AXISignal(AxLEN_WIDTH) > arlen;
    //sc_out<AXISignal(ID_WIDTH) > arid;
    //sc_out<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_in<bool >                             maxigp0_rvalid;
    sc_out<bool >                            maxigp0_rready;
    sc_in<sc_bv<M_AXI_HPM0_FPD_DATA_WIDTH_G> >    maxigp0_rdata;
    sc_in<sc_bv<2> >                         maxigp0_rresp;
    //sc_in<AXISignal(RUSER_WIDTH) > ruser; // UNUSED so tied-off
    sc_in<sc_bv<M_AXI_HPM0_FPD_ID_WIDTH_G> >      maxigp0_rid;
    //sc_in<AXISignal(ID_WIDTH) > rid;
    sc_in<bool >                             maxigp0_rlast;

    // Ports so that the bridge can be bound
    sc_out<sc_bv<4> > maxigp0_arregion;
    sc_out<sc_bv<4> > maxigp0_awregion;

    // M AXI GP1 Ports
    ////////////////////

    sc_in<bool>                              maxigp1_aclk;
    sc_in<bool>                              maxigp1_aresetn;

    /* Write address channel.  */
    sc_out<bool >                            maxigp1_awvalid;
    sc_in<bool >                             maxigp1_awready;
    sc_out<sc_bv<M_AXI_HPM1_FPD_ADDR_WIDTH_G> >   maxigp1_awaddr;
    sc_out<sc_bv<3> >                        maxigp1_awprot;
    sc_out<sc_bv<4> >                        maxigp1_awqos;
    sc_out<sc_bv<4> >                        maxigp1_awcache;
    sc_out<sc_bv<2> >                        maxigp1_awburst;
    sc_out<sc_bv<3> >                        maxigp1_awsize;
    sc_out<sc_bv<M_AXI_HPM1_FPD_AXLEN_WIDTH_G> >  maxigp1_awlen;
    sc_out<sc_bv<M_AXI_HPM1_FPD_ID_WIDTH_G> >     maxigp1_awid;
    //sc_out<sc_bv<M_AXI_HPM1_FPD_AXLOCK_WIDTH_G> > maxigp1_awlock;
    sc_out<bool>                                    maxigp1_awlock;
    // BFM side is shown below : so some modification may be needed for these signals
    // Each time BFM side differs is shown in comments below
    //sc_out<AXISignal(AxLEN_WIDTH) > awlen;
    //sc_out<AXISignal(ID_WIDTH) > awid;
    //sc_out<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_out<sc_bv<M_AXI_HPM1_FPD_ID_WIDTH_G> >     maxigp1_wid;
    //sc_out<AXISignal(ID_WIDTH) > wid;
    sc_out<bool >                            maxigp1_wvalid;
    sc_in<bool >                             maxigp1_wready;
    sc_out<sc_bv<M_AXI_HPM1_FPD_DATA_WIDTH_G> >   maxigp1_wdata;
    sc_out<sc_bv<M_AXI_HPM1_FPD_DATA_WIDTH_G/8> > maxigp1_wstrb;
    sc_out<bool >                            maxigp1_wlast;

    /* Write response channel.  */
    sc_in<bool >                             maxigp1_bvalid;
    sc_out<bool >                            maxigp1_bready;
    sc_in<sc_bv<2> >                         maxigp1_bresp;
    //sc_in<AXISignal(BUSER_WIDTH) > buser; // UNUSED so tied-off
    sc_in<sc_bv<M_AXI_HPM1_FPD_ID_WIDTH_G> >      maxigp1_bid;
    //sc_in<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_out<bool >                            maxigp1_arvalid;
    sc_in<bool >                             maxigp1_arready;
    sc_out<sc_bv<M_AXI_HPM1_FPD_ADDR_WIDTH_G> >   maxigp1_araddr;
    sc_out<sc_bv<3> >                        maxigp1_arprot;
    sc_out<sc_bv<4> >                        maxigp1_arqos;
    sc_out<sc_bv<4> >                        maxigp1_arcache;
    sc_out<sc_bv<2> >                        maxigp1_arburst;
    sc_out<sc_bv<3> >                        maxigp1_arsize;
    sc_out<sc_bv<M_AXI_HPM1_FPD_AXLEN_WIDTH_G> >  maxigp1_arlen;
    sc_out<sc_bv<M_AXI_HPM1_FPD_ID_WIDTH_G> >     maxigp1_arid;
    //sc_out<sc_bv<M_AXI_HPM1_FPD_AXLOCK_WIDTH_G> > maxigp1_arlock;
    sc_out<bool>                             maxigp1_arlock;
    //sc_out<AXISignal(AxLEN_WIDTH) > arlen;
    //sc_out<AXISignal(ID_WIDTH) > arid;
    //sc_out<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_in<bool >                             maxigp1_rvalid;
    sc_out<bool >                            maxigp1_rready;
    sc_in<sc_bv<M_AXI_HPM1_FPD_DATA_WIDTH_G> >    maxigp1_rdata;
    sc_in<sc_bv<2> >                         maxigp1_rresp;
    //sc_in<AXISignal(RUSER_WIDTH) > ruser; // UNUSED so tied-off
    sc_in<sc_bv<M_AXI_HPM1_FPD_ID_WIDTH_G> >      maxigp1_rid;
    //sc_in<AXISignal(ID_WIDTH) > rid;
    sc_in<bool >                             maxigp1_rlast;

    // Ports so that the bridge can be bound
    sc_out<sc_bv<4> > maxigp1_arregion;
    sc_out<sc_bv<4> > maxigp1_awregion;


     // M AXI GP2 Ports
    ////////////////////

    sc_in<bool>                              maxigp2_aclk;
    sc_in<bool>                              maxigp2_aresetn;

    /* Write address channel.  */
    sc_out<bool >                                   maxigp2_awvalid;
    sc_in<bool >                                    maxigp2_awready;
    sc_out<sc_bv<M_AXI_HPM0_LPD_ADDR_WIDTH_G> >     maxigp2_awaddr;
    sc_out<sc_bv<3> >                               maxigp2_awprot;
    sc_out<sc_bv<4> >                               maxigp2_awqos;
    sc_out<sc_bv<4> >                               maxigp2_awcache;
    sc_out<sc_bv<2> >                               maxigp2_awburst;
    sc_out<sc_bv<3> >                               maxigp2_awsize;
    sc_out<sc_bv<M_AXI_HPM0_LPD_AXLEN_WIDTH_G> >    maxigp2_awlen;
    sc_out<sc_bv<M_AXI_HPM0_LPD_ID_WIDTH_G> >       maxigp2_awid;
    //sc_out<sc_bv<M_AXI_HPM0_LPD_AXLOCK_WIDTH_G> > maxigp2_awlock;
    sc_out<bool>                                    maxigp2_awlock;

    // BFM side is shown below : so some modification may be needed for these signals
    // Each time BFM side differs is shown in comments below
    //sc_out<AXISignal(AxLEN_WIDTH) > awlen;
    //sc_out<AXISignal(ID_WIDTH) > awid;
    //sc_out<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_out<sc_bv<M_AXI_HPM0_LPD_ID_WIDTH_G> >     maxigp2_wid;
    //sc_out<AXISignal(ID_WIDTH) > wid;
    sc_out<bool >                            maxigp2_wvalid;
    sc_in<bool >                             maxigp2_wready;
    sc_out<sc_bv<M_AXI_HPM0_LPD_DATA_WIDTH_G> >   maxigp2_wdata;
    sc_out<sc_bv<M_AXI_HPM0_LPD_DATA_WIDTH_G/8> > maxigp2_wstrb;
    sc_out<bool >                            maxigp2_wlast;

    /* Write response channel.  */
    sc_in<bool >                             maxigp2_bvalid;
    sc_out<bool >                            maxigp2_bready;
    sc_in<sc_bv<2> >                         maxigp2_bresp;
    //sc_in<AXISignal(BUSER_WIDTH) > buser; // UNUSED so tied-off
    sc_in<sc_bv<M_AXI_HPM0_LPD_ID_WIDTH_G> >      maxigp2_bid;
    //sc_in<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_out<bool >                            maxigp2_arvalid;
    sc_in<bool >                             maxigp2_arready;
    sc_out<sc_bv<M_AXI_HPM0_LPD_ADDR_WIDTH_G> >   maxigp2_araddr;
    sc_out<sc_bv<3> >                        maxigp2_arprot;
    sc_out<sc_bv<4> >                        maxigp2_arqos;
    sc_out<sc_bv<4> >                        maxigp2_arcache;
    sc_out<sc_bv<2> >                        maxigp2_arburst;
    sc_out<sc_bv<3> >                        maxigp2_arsize;
    sc_out<sc_bv<M_AXI_HPM0_LPD_AXLEN_WIDTH_G> >  maxigp2_arlen;
    sc_out<sc_bv<M_AXI_HPM0_LPD_ID_WIDTH_G> >     maxigp2_arid;
    //sc_out<sc_bv<M_AXI_HPM0_LPD_AXLOCK_WIDTH_G> > maxigp2_arlock;
    sc_out<bool>                             maxigp2_arlock;
    //sc_out<AXISignal(AxLEN_WIDTH) > arlen;
    //sc_out<AXISignal(ID_WIDTH) > arid;
    //sc_out<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_in<bool >                             maxigp2_rvalid;
    sc_out<bool >                            maxigp2_rready;
    sc_in<sc_bv<M_AXI_HPM0_LPD_DATA_WIDTH_G> >    maxigp2_rdata;
    sc_in<sc_bv<2> >                         maxigp2_rresp;
    //sc_in<AXISignal(RUSER_WIDTH) > ruser; // UNUSED so tied-off
    sc_in<sc_bv<M_AXI_HPM0_LPD_ID_WIDTH_G> >      maxigp2_rid;
    //sc_in<AXISignal(ID_WIDTH) > rid;
    sc_in<bool >                             maxigp2_rlast;

    // Ports so that the bridge can be bound
    sc_out<sc_bv<4> > maxigp2_arregion;
    sc_out<sc_bv<4> > maxigp2_awregion;


    // S AXI GP0 Ports
    ////////////////////
    sc_in<bool>                              saxigp0_aclk;
    sc_in<bool>                              saxigp0_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              saxigp0_awvalid;
    sc_out<bool>                             saxigp0_awready;
    sc_in<sc_bv<S_AXI_HPC0_FPD_ADDR_WIDTH_G> >    saxigp0_awaddr;
    sc_in<sc_bv<3> >                         saxigp0_awprot;
    //sc_in<sc_bv<S_AXI_HPC0_FPD_AWUSER_WIDTH_G> >  saxigp0_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         saxigp0_awregion;
    sc_in<sc_bv<4> >                         saxigp0_awqos;
    sc_in<sc_bv<4> >                         saxigp0_awcache;
    sc_in<sc_bv<2> >                         saxigp0_awburst;
    sc_in<sc_bv<3> >                         saxigp0_awsize;
    sc_in<sc_bv<S_AXI_HPC0_FPD_AXLEN_WIDTH_G> >   saxigp0_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_HPC0_FPD_ID_WIDTH_G> >      saxigp0_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    //sc_in<sc_bv<S_AXI_HPC0_FPD_AXLOCK_WIDTH_G> >  saxigp0_awlock;
    sc_in<bool>                                    saxigp0_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_HPC0_FPD_ID_WIDTH_G> >      saxigp0_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              saxigp0_wvalid;
    sc_out<bool>                             saxigp0_wready;
    sc_in<sc_bv<S_AXI_HPC0_FPD_DATA_WIDTH_G> >    saxigp0_wdata;
    sc_in<sc_bv<S_AXI_HPC0_FPD_DATA_WIDTH_G/8> >  saxigp0_wstrb;
    //sc_in<sc_bv<2> >                         saxigp0_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              saxigp0_wlast;

    /* Write response channel.  */
    sc_out<bool>                             saxigp0_bvalid;
    sc_in<bool>                              saxigp0_bready;
    sc_out<sc_bv<2> >                        saxigp0_bresp;
    //sc_out<sc_bv<2> >                        saxigp0_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_HPC0_FPD_ID_WIDTH_G> >     saxigp0_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              saxigp0_arvalid;
    sc_out<bool>                             saxigp0_arready;
    sc_in<sc_bv<S_AXI_HPC0_FPD_ADDR_WIDTH_G> >    saxigp0_araddr;
    sc_in<sc_bv<3> >                         saxigp0_arprot;
    //sc_in<sc_bv<S_AXI_HPC0_FPD_ARUSER_WIDTH_G> >  saxigp0_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         saxigp0_arregion;
    sc_in<sc_bv<4> >                         saxigp0_arqos;
    sc_in<sc_bv<4> >                         saxigp0_arcache;
    sc_in<sc_bv<2> >                         saxigp0_arburst;
    sc_in<sc_bv<3> >                         saxigp0_arsize;
    sc_in<sc_bv<S_AXI_HPC0_FPD_AXLEN_WIDTH_G> >   saxigp0_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_HPC0_FPD_ID_WIDTH_G> >      saxigp0_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    //sc_in<sc_bv<S_AXI_HPC0_FPD_AXLOCK_WIDTH_G> >  saxigp0_arlock;
    sc_in<bool>                             saxigp0_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             saxigp0_rvalid;
    sc_in<bool>                              saxigp0_rready;
    sc_out<sc_bv<S_AXI_HPC0_FPD_DATA_WIDTH_G> >   saxigp0_rdata;
    sc_out<sc_bv<2> >                        saxigp0_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        saxigp0_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_HPC0_FPD_ID_WIDTH_G> >     saxigp0_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             saxigp0_rlast;

    // S AXI GP1 Ports
    ////////////////////
    sc_in<bool>                              saxigp1_aclk;
    sc_in<bool>                              saxigp1_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              saxigp1_awvalid;
    sc_out<bool>                             saxigp1_awready;
    sc_in<sc_bv<S_AXI_HPC1_FPD_ADDR_WIDTH_G> >    saxigp1_awaddr;
    sc_in<sc_bv<3> >                         saxigp1_awprot;
    //sc_in<sc_bv<S_AXI_HPC1_FPD_AWUSER_WIDTH_G> >  saxigp1_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         saxigp1_awregion;
    sc_in<sc_bv<4> >                         saxigp1_awqos;
    sc_in<sc_bv<4> >                         saxigp1_awcache;
    sc_in<sc_bv<2> >                         saxigp1_awburst;
    sc_in<sc_bv<3> >                         saxigp1_awsize;
    sc_in<sc_bv<S_AXI_HPC1_FPD_AXLEN_WIDTH_G> >   saxigp1_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_HPC1_FPD_ID_WIDTH_G> >      saxigp1_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    //sc_in<sc_bv<S_AXI_HPC1_FPD_AXLOCK_WIDTH_G> >  saxigp1_awlock;
    sc_in<bool>                                    saxigp1_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_HPC1_FPD_ID_WIDTH_G> >      saxigp1_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              saxigp1_wvalid;
    sc_out<bool>                             saxigp1_wready;
    sc_in<sc_bv<S_AXI_HPC1_FPD_DATA_WIDTH_G> >    saxigp1_wdata;
    sc_in<sc_bv<S_AXI_HPC1_FPD_DATA_WIDTH_G/8> >  saxigp1_wstrb;
    //sc_in<sc_bv<2> >                         saxigp1_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              saxigp1_wlast;

    /* Write response channel.  */
    sc_out<bool>                             saxigp1_bvalid;
    sc_in<bool>                              saxigp1_bready;
    sc_out<sc_bv<2> >                        saxigp1_bresp;
    //sc_out<sc_bv<2> >                        saxigp1_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_HPC1_FPD_ID_WIDTH_G> >     saxigp1_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              saxigp1_arvalid;
    sc_out<bool>                             saxigp1_arready;
    sc_in<sc_bv<S_AXI_HPC1_FPD_ADDR_WIDTH_G> >    saxigp1_araddr;
    sc_in<sc_bv<3> >                         saxigp1_arprot;
    //sc_in<sc_bv<S_AXI_HPC1_FPD_ARUSER_WIDTH_G> >  saxigp1_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         saxigp1_arregion;
    sc_in<sc_bv<4> >                         saxigp1_arqos;
    sc_in<sc_bv<4> >                         saxigp1_arcache;
    sc_in<sc_bv<2> >                         saxigp1_arburst;
    sc_in<sc_bv<3> >                         saxigp1_arsize;
    sc_in<sc_bv<S_AXI_HPC1_FPD_AXLEN_WIDTH_G> >   saxigp1_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_HPC1_FPD_ID_WIDTH_G> >      saxigp1_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    //sc_in<sc_bv<S_AXI_HPC1_FPD_AXLOCK_WIDTH_G> >  saxigp1_arlock;
    sc_in<bool>                             saxigp1_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             saxigp1_rvalid;
    sc_in<bool>                              saxigp1_rready;
    sc_out<sc_bv<S_AXI_HPC1_FPD_DATA_WIDTH_G> >   saxigp1_rdata;
    sc_out<sc_bv<2> >                        saxigp1_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        saxigp1_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_HPC1_FPD_ID_WIDTH_G> >     saxigp1_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             saxigp1_rlast;

    // S AXI HP0 Ports
    ////////////////////

    sc_in<bool>                              saxigp2_aclk;
    sc_in<bool>                              saxigp2_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              saxigp2_awvalid;
    sc_out<bool>                             saxigp2_awready;
    sc_in<sc_bv<S_AXI_HP0_FPD_ADDR_WIDTH_G> >    saxigp2_awaddr;
    sc_in<sc_bv<3> >                         saxigp2_awprot;
    //sc_in<sc_bv<S_AXI_HPC0_FPD_AWUSER_WIDTH_G> >  saxigp2_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         saxigp2_awregion;
    sc_in<sc_bv<4> >                         saxigp2_awqos;
    sc_in<sc_bv<4> >                         saxigp2_awcache;
    sc_in<sc_bv<2> >                         saxigp2_awburst;
    sc_in<sc_bv<3> >                         saxigp2_awsize;
    sc_in<sc_bv<S_AXI_HP0_FPD_AXLEN_WIDTH_G> >   saxigp2_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_HP0_FPD_ID_WIDTH_G> >      saxigp2_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    //sc_in<sc_bv<S_AXI_HPC0_FPD_AXLOCK_WIDTH_G> >  saxigp2_awlock;
    sc_in<bool>                                    saxigp2_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_HP0_FPD_ID_WIDTH_G> >      saxigp2_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              saxigp2_wvalid;
    sc_out<bool>                             saxigp2_wready;
    sc_in<sc_bv<S_AXI_HP0_FPD_DATA_WIDTH_G> >    saxigp2_wdata;
    sc_in<sc_bv<S_AXI_HP0_FPD_DATA_WIDTH_G/8> >  saxigp2_wstrb;
    //sc_in<sc_bv<2> >                         saxigp2_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              saxigp2_wlast;

    /* Write response channel.  */
    sc_out<bool>                             saxigp2_bvalid;
    sc_in<bool>                              saxigp2_bready;
    sc_out<sc_bv<2> >                        saxigp2_bresp;
    //sc_out<sc_bv<2> >                        saxigp2_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_HP0_FPD_ID_WIDTH_G> >     saxigp2_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              saxigp2_arvalid;
    sc_out<bool>                             saxigp2_arready;
    sc_in<sc_bv<S_AXI_HP0_FPD_ADDR_WIDTH_G> >    saxigp2_araddr;
    sc_in<sc_bv<3> >                         saxigp2_arprot;
    //sc_in<sc_bv<S_AXI_HPC0_FPD_ARUSER_WIDTH_G> >  saxigp2_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         saxigp2_arregion;
    sc_in<sc_bv<4> >                         saxigp2_arqos;
    sc_in<sc_bv<4> >                         saxigp2_arcache;
    sc_in<sc_bv<2> >                         saxigp2_arburst;
    sc_in<sc_bv<3> >                         saxigp2_arsize;
    sc_in<sc_bv<S_AXI_HP0_FPD_AXLEN_WIDTH_G> >   saxigp2_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_HP0_FPD_ID_WIDTH_G> >      saxigp2_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    //sc_in<sc_bv<S_AXI_HPC0_FPD_AXLOCK_WIDTH_G> >  saxigp2_arlock;
    sc_in<bool>                             saxigp2_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             saxigp2_rvalid;
    sc_in<bool>                              saxigp2_rready;
    sc_out<sc_bv<S_AXI_HP0_FPD_DATA_WIDTH_G> >   saxigp2_rdata;
    sc_out<sc_bv<2> >                        saxigp2_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        saxigp2_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_HP0_FPD_ID_WIDTH_G> >     saxigp2_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             saxigp2_rlast;

    // S AXI HP1 Ports
    ////////////////////

    sc_in<bool>                              saxigp3_aclk;
    sc_in<bool>                              saxigp3_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              saxigp3_awvalid;
    sc_out<bool>                             saxigp3_awready;
    sc_in<sc_bv<S_AXI_HP1_FPD_ADDR_WIDTH_G> >    saxigp3_awaddr;
    sc_in<sc_bv<3> >                         saxigp3_awprot;
    //sc_in<sc_bv<S_AXI_HP1_FPD_AWUSER_WIDTH_G> >  saxigp3_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         saxigp3_awregion;
    sc_in<sc_bv<4> >                         saxigp3_awqos;
    sc_in<sc_bv<4> >                         saxigp3_awcache;
    sc_in<sc_bv<2> >                         saxigp3_awburst;
    sc_in<sc_bv<3> >                         saxigp3_awsize;
    sc_in<sc_bv<S_AXI_HP1_FPD_AXLEN_WIDTH_G> >   saxigp3_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_HP1_FPD_ID_WIDTH_G> >      saxigp3_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    //sc_in<sc_bv<S_AXI_HP1_FPD_AXLOCK_WIDTH_G> >  saxigp3_awlock;
    sc_in<bool>                                    saxigp3_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_HP1_FPD_ID_WIDTH_G> >      saxigp3_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              saxigp3_wvalid;
    sc_out<bool>                             saxigp3_wready;
    sc_in<sc_bv<S_AXI_HP1_FPD_DATA_WIDTH_G> >    saxigp3_wdata;
    sc_in<sc_bv<S_AXI_HP1_FPD_DATA_WIDTH_G/8> >  saxigp3_wstrb;
    //sc_in<sc_bv<2> >                         saxigp3_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              saxigp3_wlast;

    /* Write response channel.  */
    sc_out<bool>                             saxigp3_bvalid;
    sc_in<bool>                              saxigp3_bready;
    sc_out<sc_bv<2> >                        saxigp3_bresp;
    //sc_out<sc_bv<2> >                        saxigp3_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_HP1_FPD_ID_WIDTH_G> >     saxigp3_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              saxigp3_arvalid;
    sc_out<bool>                             saxigp3_arready;
    sc_in<sc_bv<S_AXI_HP1_FPD_ADDR_WIDTH_G> >    saxigp3_araddr;
    sc_in<sc_bv<3> >                         saxigp3_arprot;
    //sc_in<sc_bv<S_AXI_HP1_FPD_ARUSER_WIDTH_G> >  saxigp3_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         saxigp3_arregion;
    sc_in<sc_bv<4> >                         saxigp3_arqos;
    sc_in<sc_bv<4> >                         saxigp3_arcache;
    sc_in<sc_bv<2> >                         saxigp3_arburst;
    sc_in<sc_bv<3> >                         saxigp3_arsize;
    sc_in<sc_bv<S_AXI_HP1_FPD_AXLEN_WIDTH_G> >   saxigp3_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_HP1_FPD_ID_WIDTH_G> >      saxigp3_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    //sc_in<sc_bv<S_AXI_HP1_FPD_AXLOCK_WIDTH_G> >  saxigp3_arlock;
    sc_in<bool>                             saxigp3_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             saxigp3_rvalid;
    sc_in<bool>                              saxigp3_rready;
    sc_out<sc_bv<S_AXI_HP1_FPD_DATA_WIDTH_G> >   saxigp3_rdata;
    sc_out<sc_bv<2> >                        saxigp3_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        saxigp3_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_HP1_FPD_ID_WIDTH_G> >     saxigp3_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             saxigp3_rlast;

    // S AXI HP2 Ports
    ////////////////////

    sc_in<bool>                              saxigp4_aclk;
    sc_in<bool>                              saxigp4_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              saxigp4_awvalid;
    sc_out<bool>                             saxigp4_awready;
    sc_in<sc_bv<S_AXI_HP2_FPD_ADDR_WIDTH_G> >    saxigp4_awaddr;
    sc_in<sc_bv<3> >                         saxigp4_awprot;
    //sc_in<sc_bv<S_AXI_HP2_FPD_AWUSER_WIDTH_G> >  saxigp4_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         saxigp4_awregion;
    sc_in<sc_bv<4> >                         saxigp4_awqos;
    sc_in<sc_bv<4> >                         saxigp4_awcache;
    sc_in<sc_bv<2> >                         saxigp4_awburst;
    sc_in<sc_bv<3> >                         saxigp4_awsize;
    sc_in<sc_bv<S_AXI_HP2_FPD_AXLEN_WIDTH_G> >   saxigp4_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_HP2_FPD_ID_WIDTH_G> >      saxigp4_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    //sc_in<sc_bv<S_AXI_HP2_FPD_AXLOCK_WIDTH_G> >  saxigp4_awlock;
    sc_in<bool>                                    saxigp4_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_HP2_FPD_ID_WIDTH_G> >      saxigp4_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              saxigp4_wvalid;
    sc_out<bool>                             saxigp4_wready;
    sc_in<sc_bv<S_AXI_HP2_FPD_DATA_WIDTH_G> >    saxigp4_wdata;
    sc_in<sc_bv<S_AXI_HP2_FPD_DATA_WIDTH_G/8> >  saxigp4_wstrb;
    //sc_in<sc_bv<2> >                         saxigp4_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              saxigp4_wlast;

    /* Write response channel.  */
    sc_out<bool>                             saxigp4_bvalid;
    sc_in<bool>                              saxigp4_bready;
    sc_out<sc_bv<2> >                        saxigp4_bresp;
    //sc_out<sc_bv<2> >                        saxigp4_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_HP2_FPD_ID_WIDTH_G> >     saxigp4_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              saxigp4_arvalid;
    sc_out<bool>                             saxigp4_arready;
    sc_in<sc_bv<S_AXI_HP2_FPD_ADDR_WIDTH_G> >    saxigp4_araddr;
    sc_in<sc_bv<3> >                         saxigp4_arprot;
    //sc_in<sc_bv<S_AXI_HP2_FPD_ARUSER_WIDTH_G> >  saxigp4_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         saxigp4_arregion;
    sc_in<sc_bv<4> >                         saxigp4_arqos;
    sc_in<sc_bv<4> >                         saxigp4_arcache;
    sc_in<sc_bv<2> >                         saxigp4_arburst;
    sc_in<sc_bv<3> >                         saxigp4_arsize;
    sc_in<sc_bv<S_AXI_HP2_FPD_AXLEN_WIDTH_G> >   saxigp4_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_HP2_FPD_ID_WIDTH_G> >      saxigp4_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    //sc_in<sc_bv<S_AXI_HP2_FPD_AXLOCK_WIDTH_G> >  saxigp4_arlock;
    sc_in<bool>                             saxigp4_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             saxigp4_rvalid;
    sc_in<bool>                              saxigp4_rready;
    sc_out<sc_bv<S_AXI_HP2_FPD_DATA_WIDTH_G> >   saxigp4_rdata;
    sc_out<sc_bv<2> >                        saxigp4_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        saxigp4_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_HP2_FPD_ID_WIDTH_G> >     saxigp4_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             saxigp4_rlast;

    // S AXI HP3 Ports
    ////////////////////

    sc_in<bool>                              saxigp5_aclk;
    sc_in<bool>                              saxigp5_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              saxigp5_awvalid;
    sc_out<bool>                             saxigp5_awready;
    sc_in<sc_bv<S_AXI_HP3_FPD_ADDR_WIDTH_G> >    saxigp5_awaddr;
    sc_in<sc_bv<3> >                         saxigp5_awprot;
    //sc_in<sc_bv<S_AXI_HP3_FPD_AWUSER_WIDTH_G> >  saxigp5_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         saxigp5_awregion;
    sc_in<sc_bv<4> >                         saxigp5_awqos;
    sc_in<sc_bv<4> >                         saxigp5_awcache;
    sc_in<sc_bv<2> >                         saxigp5_awburst;
    sc_in<sc_bv<3> >                         saxigp5_awsize;
    sc_in<sc_bv<S_AXI_HP3_FPD_AXLEN_WIDTH_G> >   saxigp5_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_HP3_FPD_ID_WIDTH_G> >      saxigp5_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    //sc_in<sc_bv<S_AXI_HP3_FPD_AXLOCK_WIDTH_G> >  saxigp5_awlock;
    sc_in<bool>                                    saxigp5_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_HP3_FPD_ID_WIDTH_G> >      saxigp5_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              saxigp5_wvalid;
    sc_out<bool>                             saxigp5_wready;
    sc_in<sc_bv<S_AXI_HP3_FPD_DATA_WIDTH_G> >    saxigp5_wdata;
    sc_in<sc_bv<S_AXI_HP3_FPD_DATA_WIDTH_G/8> >  saxigp5_wstrb;
    //sc_in<sc_bv<2> >                         saxigp5_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              saxigp5_wlast;

    /* Write response channel.  */
    sc_out<bool>                             saxigp5_bvalid;
    sc_in<bool>                              saxigp5_bready;
    sc_out<sc_bv<2> >                        saxigp5_bresp;
    //sc_out<sc_bv<2> >                        saxigp5_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_HP3_FPD_ID_WIDTH_G> >     saxigp5_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              saxigp5_arvalid;
    sc_out<bool>                             saxigp5_arready;
    sc_in<sc_bv<S_AXI_HP3_FPD_ADDR_WIDTH_G> >    saxigp5_araddr;
    sc_in<sc_bv<3> >                         saxigp5_arprot;
    //sc_in<sc_bv<S_AXI_HP3_FPD_ARUSER_WIDTH_G> >  saxigp5_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         saxigp5_arregion;
    sc_in<sc_bv<4> >                         saxigp5_arqos;
    sc_in<sc_bv<4> >                         saxigp5_arcache;
    sc_in<sc_bv<2> >                         saxigp5_arburst;
    sc_in<sc_bv<3> >                         saxigp5_arsize;
    sc_in<sc_bv<S_AXI_HP3_FPD_AXLEN_WIDTH_G> >   saxigp5_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_HP3_FPD_ID_WIDTH_G> >      saxigp5_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    //sc_in<sc_bv<S_AXI_HP3_FPD_AXLOCK_WIDTH_G> >  saxigp5_arlock;
    sc_in<bool>                             saxigp5_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             saxigp5_rvalid;
    sc_in<bool>                              saxigp5_rready;
    sc_out<sc_bv<S_AXI_HP3_FPD_DATA_WIDTH_G> >   saxigp5_rdata;
    sc_out<sc_bv<2> >                        saxigp5_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        saxigp5_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_HP3_FPD_ID_WIDTH_G> >     saxigp5_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             saxigp5_rlast;



    // S AXI LPD Ports
    ////////////////////

    sc_in<bool>                              saxigp6_aclk;
    sc_in<bool>                              saxigp6_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              saxigp6_awvalid;
    sc_out<bool>                             saxigp6_awready;
    sc_in<sc_bv<S_AXI_LPD_ADDR_WIDTH_G> >    saxigp6_awaddr;
    sc_in<sc_bv<3> >                         saxigp6_awprot;
    //sc_in<sc_bv<S_AXI_HP3_FPD_AWUSER_WIDTH_G> >  saxigp6_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         saxigp6_awregion;
    sc_in<sc_bv<4> >                         saxigp6_awqos;
    sc_in<sc_bv<4> >                         saxigp6_awcache;
    sc_in<sc_bv<2> >                         saxigp6_awburst;
    sc_in<sc_bv<3> >                         saxigp6_awsize;
    sc_in<sc_bv<S_AXI_LPD_AXLEN_WIDTH_G> >   saxigp6_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_LPD_ID_WIDTH_G> >      saxigp6_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    //sc_in<sc_bv<S_AXI_LPD_AXLOCK_WIDTH_G> >  saxigp6_awlock;
    sc_in<bool>                                    saxigp6_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_LPD_ID_WIDTH_G> >      saxigp6_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              saxigp6_wvalid;
    sc_out<bool>                             saxigp6_wready;
    sc_in<sc_bv<S_AXI_LPD_DATA_WIDTH_G> >    saxigp6_wdata;
    sc_in<sc_bv<S_AXI_LPD_DATA_WIDTH_G/8> >  saxigp6_wstrb;
    //sc_in<sc_bv<2> >                         saxigp6_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              saxigp6_wlast;

    /* Write response channel.  */
    sc_out<bool>                             saxigp6_bvalid;
    sc_in<bool>                              saxigp6_bready;
    sc_out<sc_bv<2> >                        saxigp6_bresp;
    //sc_out<sc_bv<2> >                        saxigp6_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_LPD_ID_WIDTH_G> >     saxigp6_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              saxigp6_arvalid;
    sc_out<bool>                             saxigp6_arready;
    sc_in<sc_bv<S_AXI_LPD_ADDR_WIDTH_G> >    saxigp6_araddr;
    sc_in<sc_bv<3> >                         saxigp6_arprot;
    //sc_in<sc_bv<S_AXI_LPD_ARUSER_WIDTH_G> >  saxigp6_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         saxigp6_arregion;
    sc_in<sc_bv<4> >                         saxigp6_arqos;
    sc_in<sc_bv<4> >                         saxigp6_arcache;
    sc_in<sc_bv<2> >                         saxigp6_arburst;
    sc_in<sc_bv<3> >                         saxigp6_arsize;
    sc_in<sc_bv<S_AXI_LPD_AXLEN_WIDTH_G> >   saxigp6_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_LPD_ID_WIDTH_G> >      saxigp6_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    //sc_in<sc_bv<S_AXI_LPD_AXLOCK_WIDTH_G> >  saxigp6_arlock;
    sc_in<bool>                             saxigp6_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             saxigp6_rvalid;
    sc_in<bool>                              saxigp6_rready;
    sc_out<sc_bv<S_AXI_LPD_DATA_WIDTH_G> >   saxigp6_rdata;
    sc_out<sc_bv<2> >                        saxigp6_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        saxigp6_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_LPD_ID_WIDTH_G> >     saxigp6_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             saxigp6_rlast;


    // S AXI ACP FPD Ports
    ////////////////////

    sc_in<bool>                              saxiacp_aclk;
    sc_in<bool>                              saxiacp_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              saxiacp_awvalid;
    sc_out<bool>                             saxiacp_awready;
    sc_in<sc_bv<S_AXI_ACP_FPD_ADDR_WIDTH_G> >    saxiacp_awaddr;
    sc_in<sc_bv<3> >                         saxiacp_awprot;
    //sc_in<sc_bv<S_AXI_HP3_FPD_AWUSER_WIDTH_G> >  saxiacp_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         saxiacp_awregion;
    sc_in<sc_bv<4> >                         saxiacp_awqos;
    sc_in<sc_bv<4> >                         saxiacp_awcache;
    sc_in<sc_bv<2> >                         saxiacp_awburst;
    sc_in<sc_bv<3> >                         saxiacp_awsize;
    sc_in<sc_bv<S_AXI_ACP_FPD_AXLEN_WIDTH_G> >   saxiacp_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_ACP_FPD_ID_WIDTH_G> >      saxiacp_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    //sc_in<sc_bv<S_AXI_ACP_FPD_AXLOCK_WIDTH_G> >  saxiacp_awlock;
    sc_in<bool>                                    saxiacp_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_ACP_FPD_ID_WIDTH_G> >      saxiacp_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              saxiacp_wvalid;
    sc_out<bool>                             saxiacp_wready;
    sc_in<sc_bv<S_AXI_ACP_FPD_DATA_WIDTH_G> >    saxiacp_wdata;
    sc_in<sc_bv<S_AXI_ACP_FPD_DATA_WIDTH_G/8> >  saxiacp_wstrb;
    //sc_in<sc_bv<2> >                         saxiacp_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              saxiacp_wlast;

    /* Write response channel.  */
    sc_out<bool>                             saxiacp_bvalid;
    sc_in<bool>                              saxiacp_bready;
    sc_out<sc_bv<2> >                        saxiacp_bresp;
    //sc_out<sc_bv<2> >                        saxiacp_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_ACP_FPD_ID_WIDTH_G> >     saxiacp_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              saxiacp_arvalid;
    sc_out<bool>                             saxiacp_arready;
    sc_in<sc_bv<S_AXI_ACP_FPD_ADDR_WIDTH_G> >    saxiacp_araddr;
    sc_in<sc_bv<3> >                         saxiacp_arprot;
    //sc_in<sc_bv<S_AXI_ACP_FPD_ARUSER_WIDTH_G> >  saxiacp_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         saxiacp_arregion;
    sc_in<sc_bv<4> >                         saxiacp_arqos;
    sc_in<sc_bv<4> >                         saxiacp_arcache;
    sc_in<sc_bv<2> >                         saxiacp_arburst;
    sc_in<sc_bv<3> >                         saxiacp_arsize;
    sc_in<sc_bv<S_AXI_ACP_FPD_AXLEN_WIDTH_G> >   saxiacp_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_ACP_FPD_ID_WIDTH_G> >      saxiacp_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    //sc_in<sc_bv<S_AXI_ACP_FPD_AXLOCK_WIDTH_G> >  saxiacp_arlock;
    sc_in<bool>                             saxiacp_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             saxiacp_rvalid;
    sc_in<bool>                              saxiacp_rready;
    sc_out<sc_bv<S_AXI_ACP_FPD_DATA_WIDTH_G> >   saxiacp_rdata;
    sc_out<sc_bv<2> >                        saxiacp_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        saxiacp_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_ACP_FPD_ID_WIDTH_G> >     saxiacp_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             saxiacp_rlast;



    // S AXI ACE FPD Ports
    ////////////////////

    sc_in<bool>                              sacefpd_aclk;
    sc_in<bool>                              sacefpd_aresetn;

    /* Write address channel.  */
    sc_in<bool>                              sacefpd_awvalid;
    sc_out<bool>                             sacefpd_awready;
    sc_in<sc_bv<S_AXI_ACE_FPD_ADDR_WIDTH_G> >    sacefpd_awaddr;
    sc_in<sc_bv<3> >                         sacefpd_awprot;
    //sc_in<sc_bv<S_AXI_HP3_FPD_AWUSER_WIDTH_G> >  sacefpd_awuser;
    //sc_in<AXISignal(AWUSER_WIDTH) > awuser;
    sc_in<sc_bv<4> >                         sacefpd_awregion;
    sc_in<sc_bv<4> >                         sacefpd_awqos;
    sc_in<sc_bv<4> >                         sacefpd_awcache;
    sc_in<sc_bv<2> >                         sacefpd_awburst;
    sc_in<sc_bv<3> >                         sacefpd_awsize;
    sc_in<sc_bv<S_AXI_ACE_FPD_AXLEN_WIDTH_G> >   sacefpd_awlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > awlen;
    sc_in<sc_bv<S_AXI_ACE_FPD_ID_WIDTH_G> >      sacefpd_awid;
    //sc_in<AXISignal(ID_WIDTH) > awid;
    //sc_in<sc_bv<S_AXI_ACE_FPD_AXLOCK_WIDTH_G> >  sacefpd_awlock;
    sc_in<bool>                                    sacefpd_awlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > awlock;

    /* Write data channel.  */
    //sc_in<sc_bv<S_AXI_ACE_FPD_ID_WIDTH_G> >      sacefpd_wid;
    //sc_in<AXISignal(ID_WIDTH) > wid;
    sc_in<bool>                              sacefpd_wvalid;
    sc_out<bool>                             sacefpd_wready;
    sc_in<sc_bv<S_AXI_ACE_FPD_DATA_WIDTH_G> >    sacefpd_wdata;
    sc_in<sc_bv<S_AXI_ACE_FPD_DATA_WIDTH_G/8> >  sacefpd_wstrb;
    //sc_in<sc_bv<2> >                         sacefpd_wuser;
    //sc_in<AXISignal(WUSER_WIDTH) > wuser;
    sc_in<bool>                              sacefpd_wlast;

    /* Write response channel.  */
    sc_out<bool>                             sacefpd_bvalid;
    sc_in<bool>                              sacefpd_bready;
    sc_out<sc_bv<2> >                        sacefpd_bresp;
    //sc_out<sc_bv<2> >                        sacefpd_buser;
    //sc_out<AXISignal(BUSER_WIDTH) > buser;
    sc_out<sc_bv<S_AXI_ACE_FPD_ID_WIDTH_G> >     sacefpd_bid;
    //sc_out<AXISignal(ID_WIDTH) > bid;

    /* Read address channel.  */
    sc_in<bool>                              sacefpd_arvalid;
    sc_out<bool>                             sacefpd_arready;
    sc_in<sc_bv<S_AXI_ACE_FPD_ADDR_WIDTH_G> >    sacefpd_araddr;
    sc_in<sc_bv<3> >                         sacefpd_arprot;
    //sc_in<sc_bv<S_AXI_ACE_FPD_ARUSER_WIDTH_G> >  sacefpd_aruser;
    //sc_in<AXISignal(ARUSER_WIDTH) > aruser;
    sc_in<sc_bv<4> >                         sacefpd_arregion;
    sc_in<sc_bv<4> >                         sacefpd_arqos;
    sc_in<sc_bv<4> >                         sacefpd_arcache;
    sc_in<sc_bv<2> >                         sacefpd_arburst;
    sc_in<sc_bv<3> >                         sacefpd_arsize;
    sc_in<sc_bv<S_AXI_ACE_FPD_AXLEN_WIDTH_G> >   sacefpd_arlen;
    //sc_in<AXISignal(AxLEN_WIDTH) > arlen;
    sc_in<sc_bv<S_AXI_ACE_FPD_ID_WIDTH_G> >      sacefpd_arid;
    //sc_in<AXISignal(ID_WIDTH) > arid;
    //sc_in<sc_bv<S_AXI_ACE_FPD_AXLOCK_WIDTH_G> >  sacefpd_arlock;
    sc_in<bool>                             sacefpd_arlock;
    //sc_in<AXISignal(AxLOCK_WIDTH) > arlock;

    /* Read data channel.  */
    sc_out<bool>                             sacefpd_rvalid;
    sc_in<bool>                              sacefpd_rready;
    sc_out<sc_bv<S_AXI_ACE_FPD_DATA_WIDTH_G> >   sacefpd_rdata;
    sc_out<sc_bv<4> >                        sacefpd_rresp;
    //sc_out<sc_bv<(ACE_MODE == ACE_MODE_ACE) ? 4 : 2> > rresp;
    //sc_out<sc_bv<2> >                        sacefpd_ruser;
    //sc_out<AXISignal(RUSER_WIDTH) > ruser;
    sc_out<sc_bv<S_AXI_ACE_FPD_ID_WIDTH_G> >     sacefpd_rid;
    //sc_out<AXISignal(ID_WIDTH) > rid;
    sc_out<bool>                             sacefpd_rlast;

    ///////// END AXI


    // IRQs
    /////////

    // PL -> PS IRQ
    sc_in<sc_bv<8> > pl_ps_irq0;
    sc_in<sc_bv<8> > pl_ps_irq1;

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
    void assign_irq0(void) {
        auto tmp_irq = pl_ps_irq0.read();
        for (int i = 0; i < 8; ++i) {
            bool tmp_value = tmp_irq.get_bit(i);
            zynq.pl2ps_irq[i].write(tmp_value);
        }
    }

    void assign_irq1(void) {
        auto tmp_irq = pl_ps_irq1.read();
        for (int i = 8; i < 16; ++i) {
            bool tmp_value = tmp_irq.get_bit(i);
            zynq.pl2ps_irq[i].write(tmp_value);
        }
    }

    // Generate negative reset (if needed internally)
    void gen_rst_n(void)
    {
        rst_n.write(!rst.read());
    }

    SC_HAS_PROCESS(zynqmp_ps);
    // The combination of SC_HAS_PROCESS and a normal constructor replaces the use of SC_CTOR
    // SC_HAS_PROCESS is only needed if you have SC_THREADs or SC_METHODs in your module! If there are no processes then you don't need SC_HAS_PROCESS.

    /////////////////
    // Constructor //
    /////////////////
    zynqmp_ps(sc_module_name name) :
        // Initialize generics
        ////////////////////////
        // QEMU


        // Modules
        ////////////

        // Set the path to the socket from the generic parameter
        zynq("zynq", QEMU_PATH_TO_SOCKET_G.c_str()),


        // Signals
        ////////////
        rst("rst"),
        fclk_clk0           ("fclk_clk0"),
        fclk_clk1           ("fclk_clk1"),
        fclk_clk2           ("fclk_clk2"),
        fclk_clk3           ("fclk_clk3"),

        maxigp0_aclk      ("maxigp0_aclk"),
        maxigp0_aresetn   ("maxigp0_aresetn"),
        maxigp0_awvalid   ("maxigp0_awvalid"),
        maxigp0_awready   ("maxigp0_awready"),
        maxigp0_awaddr    ("maxigp0_awaddr"),
        maxigp0_awprot    ("maxigp0_awprot"),
        maxigp0_awqos     ("maxigp0_awqos"),
        maxigp0_awcache   ("maxigp0_awcache"),
        maxigp0_awburst   ("maxigp0_awburst"),
        maxigp0_awsize    ("maxigp0_awsize"),
        maxigp0_awlen     ("maxigp0_awlen"),
        maxigp0_awid      ("maxigp0_awid"),
        maxigp0_awlock    ("maxigp0_awlock"),
        maxigp0_wvalid    ("maxigp0_wvalid"),
        maxigp0_wready    ("maxigp0_wready"),
        maxigp0_wdata     ("maxigp0_wdata"),
        maxigp0_wstrb     ("maxigp0_wstrb"),
        maxigp0_wlast     ("maxigp0_wlast"),
        maxigp0_bvalid    ("maxigp0_bvalid"),
        maxigp0_bready    ("maxigp0_bready"),
        maxigp0_bresp     ("maxigp0_bresp"),
        maxigp0_bid       ("maxigp0_bid"),
        maxigp0_arvalid   ("maxigp0_arvalid"),
        maxigp0_arready   ("maxigp0_arready"),
        maxigp0_araddr    ("maxigp0_araddr"),
        maxigp0_arprot    ("maxigp0_arprot"),
        maxigp0_arqos     ("maxigp0_arqos"),
        maxigp0_arcache   ("maxigp0_arcache"),
        maxigp0_arburst   ("maxigp0_arburst"),
        maxigp0_arsize    ("maxigp0_arsize"),
        maxigp0_arlen     ("maxigp0_arlen"),
        maxigp0_arid      ("maxigp0_arid"),
        maxigp0_arlock    ("maxigp0_arlock"),
        maxigp0_rvalid    ("maxigp0_rvalid"),
        maxigp0_rready    ("maxigp0_rready"),
        maxigp0_rdata     ("maxigp0_rdata"),
        maxigp0_rresp     ("maxigp0_rresp"),
        maxigp0_rid       ("maxigp0_rid"),
        maxigp0_rlast     ("maxigp0_rlast"),
        maxigp0_arregion  ("maxigp0_arregion"),
        maxigp0_awregion  ("maxigp0_awregion"),

        maxigp1_aclk      ("maxigp1_aclk"),
        maxigp1_aresetn   ("maxigp1_aresetn"),
        maxigp1_awvalid   ("maxigp1_awvalid"),
        maxigp1_awready   ("maxigp1_awready"),
        maxigp1_awaddr    ("maxigp1_awaddr"),
        maxigp1_awprot    ("maxigp1_awprot"),
        maxigp1_awqos     ("maxigp1_awqos"),
        maxigp1_awcache   ("maxigp1_awcache"),
        maxigp1_awburst   ("maxigp1_awburst"),
        maxigp1_awsize    ("maxigp1_awsize"),
        maxigp1_awlen     ("maxigp1_awlen"),
        maxigp1_awid      ("maxigp1_awid"),
        maxigp1_awlock    ("maxigp1_awlock"),
        maxigp1_wvalid    ("maxigp1_wvalid"),
        maxigp1_wready    ("maxigp1_wready"),
        maxigp1_wdata     ("maxigp1_wdata"),
        maxigp1_wstrb     ("maxigp1_wstrb"),
        maxigp1_wlast     ("maxigp1_wlast"),
        maxigp1_bvalid    ("maxigp1_bvalid"),
        maxigp1_bready    ("maxigp1_bready"),
        maxigp1_bresp     ("maxigp1_bresp"),
        maxigp1_bid       ("maxigp1_bid"),
        maxigp1_arvalid   ("maxigp1_arvalid"),
        maxigp1_arready   ("maxigp1_arready"),
        maxigp1_araddr    ("maxigp1_araddr"),
        maxigp1_arprot    ("maxigp1_arprot"),
        maxigp1_arqos     ("maxigp1_arqos"),
        maxigp1_arcache   ("maxigp1_arcache"),
        maxigp1_arburst   ("maxigp1_arburst"),
        maxigp1_arsize    ("maxigp1_arsize"),
        maxigp1_arlen     ("maxigp1_arlen"),
        maxigp1_arid      ("maxigp1_arid"),
        maxigp1_arlock    ("maxigp1_arlock"),
        maxigp1_rvalid    ("maxigp1_rvalid"),
        maxigp1_rready    ("maxigp1_rready"),
        maxigp1_rdata     ("maxigp1_rdata"),
        maxigp1_rresp     ("maxigp1_rresp"),
        maxigp1_rid       ("maxigp1_rid"),
        maxigp1_rlast     ("maxigp1_rlast"),
        maxigp1_arregion  ("maxigp1_arregion"),
        maxigp1_awregion  ("maxigp1_awregion"),









        maxigp2_aclk      ("maxigp2_aclk"),
        maxigp2_aresetn   ("maxigp2_aresetn"),
        maxigp2_awvalid   ("maxigp2_awvalid"),
        maxigp2_awready   ("maxigp2_awready"),
        maxigp2_awaddr    ("maxigp2_awaddr"),
        maxigp2_awprot    ("maxigp2_awprot"),
        maxigp2_awqos     ("maxigp2_awqos"),
        maxigp2_awcache   ("maxigp2_awcache"),
        maxigp2_awburst   ("maxigp2_awburst"),
        maxigp2_awsize    ("maxigp2_awsize"),
        maxigp2_awlen     ("maxigp2_awlen"),
        maxigp2_awid      ("maxigp2_awid"),
        maxigp2_awlock    ("maxigp2_awlock"),
        maxigp2_wvalid    ("maxigp2_wvalid"),
        maxigp2_wready    ("maxigp2_wready"),
        maxigp2_wdata     ("maxigp2_wdata"),
        maxigp2_wstrb     ("maxigp2_wstrb"),
        maxigp2_wlast     ("maxigp2_wlast"),
        maxigp2_bvalid    ("maxigp2_bvalid"),
        maxigp2_bready    ("maxigp2_bready"),
        maxigp2_bresp     ("maxigp2_bresp"),
        maxigp2_bid       ("maxigp2_bid"),
        maxigp2_arvalid   ("maxigp2_arvalid"),
        maxigp2_arready   ("maxigp2_arready"),
        maxigp2_araddr    ("maxigp2_araddr"),
        maxigp2_arprot    ("maxigp2_arprot"),
        maxigp2_arqos     ("maxigp2_arqos"),
        maxigp2_arcache   ("maxigp2_arcache"),
        maxigp2_arburst   ("maxigp2_arburst"),
        maxigp2_arsize    ("maxigp2_arsize"),
        maxigp2_arlen     ("maxigp2_arlen"),
        maxigp2_arid      ("maxigp2_arid"),
        maxigp2_arlock    ("maxigp2_arlock"),
        maxigp2_rvalid    ("maxigp2_rvalid"),
        maxigp2_rready    ("maxigp2_rready"),
        maxigp2_rdata     ("maxigp2_rdata"),
        maxigp2_rresp     ("maxigp2_rresp"),
        maxigp2_rid       ("maxigp2_rid"),
        maxigp2_rlast     ("maxigp2_rlast"),
        maxigp2_arregion  ("maxigp2_arregion"),
        maxigp2_awregion  ("maxigp2_awregion"),








        saxigp0_aclk      ("saxigp0_aclk"),
        saxigp0_aresetn   ("saxigp0_aresetn"),
        saxigp0_awvalid   ("saxigp0_awvalid"),
        saxigp0_awready   ("saxigp0_awready"),
        saxigp0_awaddr    ("saxigp0_awaddr"),
        saxigp0_awprot    ("saxigp0_awprot"),
        saxigp0_awregion  ("saxigp0_awregion"),
        saxigp0_awqos     ("saxigp0_awqos"),
        saxigp0_awcache   ("saxigp0_awcache"),
        saxigp0_awburst   ("saxigp0_awburst"),
        saxigp0_awsize    ("saxigp0_awsize"),
        saxigp0_awlen     ("saxigp0_awlen"),
        saxigp0_awid      ("saxigp0_awid"),
        saxigp0_awlock    ("saxigp0_awlock"),
        saxigp0_wvalid    ("saxigp0_wvalid"),
        saxigp0_wready    ("saxigp0_wready"),
        saxigp0_wdata     ("saxigp0_wdata"),
        saxigp0_wstrb     ("saxigp0_wstrb"),
        saxigp0_wlast     ("saxigp0_wlast"),
        saxigp0_bvalid    ("saxigp0_bvalid"),
        saxigp0_bready    ("saxigp0_bready"),
        saxigp0_bresp     ("saxigp0_bresp"),
        saxigp0_bid       ("saxigp0_bid"),
        saxigp0_arvalid   ("saxigp0_arvalid"),
        saxigp0_arready   ("saxigp0_arready"),
        saxigp0_araddr    ("saxigp0_araddr"),
        saxigp0_arprot    ("saxigp0_arprot"),
        saxigp0_arregion  ("saxigp0_arregion"),
        saxigp0_arqos     ("saxigp0_arqos"),
        saxigp0_arcache   ("saxigp0_arcache"),
        saxigp0_arburst   ("saxigp0_arburst"),
        saxigp0_arsize    ("saxigp0_arsize"),
        saxigp0_arlen     ("saxigp0_arlen"),
        saxigp0_arid      ("saxigp0_arid"),
        saxigp0_arlock    ("saxigp0_arlock"),
        saxigp0_rvalid    ("saxigp0_rvalid"),
        saxigp0_rready    ("saxigp0_rready"),
        saxigp0_rdata     ("saxigp0_rdata"),
        saxigp0_rresp     ("saxigp0_rresp"),
        saxigp0_rid       ("saxigp0_rid"),
        saxigp0_rlast     ("saxigp0_rlast"),

        saxigp1_aclk      ("saxigp1_aclk"),
        saxigp1_aresetn   ("saxigp1_aresetn"),
        saxigp1_awvalid   ("saxigp1_awvalid"),
        saxigp1_awready   ("saxigp1_awready"),
        saxigp1_awaddr    ("saxigp1_awaddr"),
        saxigp1_awprot    ("saxigp1_awprot"),
        saxigp1_awregion  ("saxigp1_awregion"),
        saxigp1_awqos     ("saxigp1_awqos"),
        saxigp1_awcache   ("saxigp1_awcache"),
        saxigp1_awburst   ("saxigp1_awburst"),
        saxigp1_awsize    ("saxigp1_awsize"),
        saxigp1_awlen     ("saxigp1_awlen"),
        saxigp1_awid      ("saxigp1_awid"),
        saxigp1_awlock    ("saxigp1_awlock"),
        saxigp1_wvalid    ("saxigp1_wvalid"),
        saxigp1_wready    ("saxigp1_wready"),
        saxigp1_wdata     ("saxigp1_wdata"),
        saxigp1_wstrb     ("saxigp1_wstrb"),
        saxigp1_wlast     ("saxigp1_wlast"),
        saxigp1_bvalid    ("saxigp1_bvalid"),
        saxigp1_bready    ("saxigp1_bready"),
        saxigp1_bresp     ("saxigp1_bresp"),
        saxigp1_bid       ("saxigp1_bid"),
        saxigp1_arvalid   ("saxigp1_arvalid"),
        saxigp1_arready   ("saxigp1_arready"),
        saxigp1_araddr    ("saxigp1_araddr"),
        saxigp1_arprot    ("saxigp1_arprot"),
        saxigp1_arregion  ("saxigp1_arregion"),
        saxigp1_arqos     ("saxigp1_arqos"),
        saxigp1_arcache   ("saxigp1_arcache"),
        saxigp1_arburst   ("saxigp1_arburst"),
        saxigp1_arsize    ("saxigp1_arsize"),
        saxigp1_arlen     ("saxigp1_arlen"),
        saxigp1_arid      ("saxigp1_arid"),
        saxigp1_arlock    ("saxigp1_arlock"),
        saxigp1_rvalid    ("saxigp1_rvalid"),
        saxigp1_rready    ("saxigp1_rready"),
        saxigp1_rdata     ("saxigp1_rdata"),
        saxigp1_rresp     ("saxigp1_rresp"),
        saxigp1_rid       ("saxigp1_rid"),
        saxigp1_rlast     ("saxigp1_rlast"),


        saxigp2_aclk      ("saxigp2_aclk"),
        saxigp2_aresetn   ("saxigp2_aresetn"),
        saxigp2_awvalid   ("saxigp2_awvalid"),
        saxigp2_awready   ("saxigp2_awready"),
        saxigp2_awaddr    ("saxigp2_awaddr"),
        saxigp2_awprot    ("saxigp2_awprot"),
        saxigp2_awregion  ("saxigp2_awregion"),
        saxigp2_awqos     ("saxigp2_awqos"),
        saxigp2_awcache   ("saxigp2_awcache"),
        saxigp2_awburst   ("saxigp2_awburst"),
        saxigp2_awsize    ("saxigp2_awsize"),
        saxigp2_awlen     ("saxigp2_awlen"),
        saxigp2_awid      ("saxigp2_awid"),
        saxigp2_awlock    ("saxigp2_awlock"),
        saxigp2_wvalid    ("saxigp2_wvalid"),
        saxigp2_wready    ("saxigp2_wready"),
        saxigp2_wdata     ("saxigp2_wdata"),
        saxigp2_wstrb     ("saxigp2_wstrb"),
        saxigp2_wlast     ("saxigp2_wlast"),
        saxigp2_bvalid    ("saxigp2_bvalid"),
        saxigp2_bready    ("saxigp2_bready"),
        saxigp2_bresp     ("saxigp2_bresp"),
        saxigp2_bid       ("saxigp2_bid"),
        saxigp2_arvalid   ("saxigp2_arvalid"),
        saxigp2_arready   ("saxigp2_arready"),
        saxigp2_araddr    ("saxigp2_araddr"),
        saxigp2_arprot    ("saxigp2_arprot"),
        saxigp2_arregion  ("saxigp2_arregion"),
        saxigp2_arqos     ("saxigp2_arqos"),
        saxigp2_arcache   ("saxigp2_arcache"),
        saxigp2_arburst   ("saxigp2_arburst"),
        saxigp2_arsize    ("saxigp2_arsize"),
        saxigp2_arlen     ("saxigp2_arlen"),
        saxigp2_arid      ("saxigp2_arid"),
        saxigp2_arlock    ("saxigp2_arlock"),
        saxigp2_rvalid    ("saxigp2_rvalid"),
        saxigp2_rready    ("saxigp2_rready"),
        saxigp2_rdata     ("saxigp2_rdata"),
        saxigp2_rresp     ("saxigp2_rresp"),
        saxigp2_rid       ("saxigp2_rid"),
        saxigp2_rlast     ("saxigp2_rlast"),

        saxigp3_aclk      ("saxigp3_aclk"),
        saxigp3_aresetn   ("saxigp3_aresetn"),
        saxigp3_awvalid   ("saxigp3_awvalid"),
        saxigp3_awready   ("saxigp3_awready"),
        saxigp3_awaddr    ("saxigp3_awaddr"),
        saxigp3_awprot    ("saxigp3_awprot"),
        saxigp3_awregion  ("saxigp3_awregion"),
        saxigp3_awqos     ("saxigp3_awqos"),
        saxigp3_awcache   ("saxigp3_awcache"),
        saxigp3_awburst   ("saxigp3_awburst"),
        saxigp3_awsize    ("saxigp3_awsize"),
        saxigp3_awlen     ("saxigp3_awlen"),
        saxigp3_awid      ("saxigp3_awid"),
        saxigp3_awlock    ("saxigp3_awlock"),
        saxigp3_wvalid    ("saxigp3_wvalid"),
        saxigp3_wready    ("saxigp3_wready"),
        saxigp3_wdata     ("saxigp3_wdata"),
        saxigp3_wstrb     ("saxigp3_wstrb"),
        saxigp3_wlast     ("saxigp3_wlast"),
        saxigp3_bvalid    ("saxigp3_bvalid"),
        saxigp3_bready    ("saxigp3_bready"),
        saxigp3_bresp     ("saxigp3_bresp"),
        saxigp3_bid       ("saxigp3_bid"),
        saxigp3_arvalid   ("saxigp3_arvalid"),
        saxigp3_arready   ("saxigp3_arready"),
        saxigp3_araddr    ("saxigp3_araddr"),
        saxigp3_arprot    ("saxigp3_arprot"),
        saxigp3_arregion  ("saxigp3_arregion"),
        saxigp3_arqos     ("saxigp3_arqos"),
        saxigp3_arcache   ("saxigp3_arcache"),
        saxigp3_arburst   ("saxigp3_arburst"),
        saxigp3_arsize    ("saxigp3_arsize"),
        saxigp3_arlen     ("saxigp3_arlen"),
        saxigp3_arid      ("saxigp3_arid"),
        saxigp3_arlock    ("saxigp3_arlock"),
        saxigp3_rvalid    ("saxigp3_rvalid"),
        saxigp3_rready    ("saxigp3_rready"),
        saxigp3_rdata     ("saxigp3_rdata"),
        saxigp3_rresp     ("saxigp3_rresp"),
        saxigp3_rid       ("saxigp3_rid"),
        saxigp3_rlast     ("saxigp3_rlast"),

        saxigp4_aclk      ("saxigp4_aclk"),
        saxigp4_aresetn   ("saxigp4_aresetn"),
        saxigp4_awvalid   ("saxigp4_awvalid"),
        saxigp4_awready   ("saxigp4_awready"),
        saxigp4_awaddr    ("saxigp4_awaddr"),
        saxigp4_awprot    ("saxigp4_awprot"),
        saxigp4_awregion  ("saxigp4_awregion"),
        saxigp4_awqos     ("saxigp4_awqos"),
        saxigp4_awcache   ("saxigp4_awcache"),
        saxigp4_awburst   ("saxigp4_awburst"),
        saxigp4_awsize    ("saxigp4_awsize"),
        saxigp4_awlen     ("saxigp4_awlen"),
        saxigp4_awid      ("saxigp4_awid"),
        saxigp4_awlock    ("saxigp4_awlock"),
        saxigp4_wvalid    ("saxigp4_wvalid"),
        saxigp4_wready    ("saxigp4_wready"),
        saxigp4_wdata     ("saxigp4_wdata"),
        saxigp4_wstrb     ("saxigp4_wstrb"),
        saxigp4_wlast     ("saxigp4_wlast"),
        saxigp4_bvalid    ("saxigp4_bvalid"),
        saxigp4_bready    ("saxigp4_bready"),
        saxigp4_bresp     ("saxigp4_bresp"),
        saxigp4_bid       ("saxigp4_bid"),
        saxigp4_arvalid   ("saxigp4_arvalid"),
        saxigp4_arready   ("saxigp4_arready"),
        saxigp4_araddr    ("saxigp4_araddr"),
        saxigp4_arprot    ("saxigp4_arprot"),
        saxigp4_arregion  ("saxigp4_arregion"),
        saxigp4_arqos     ("saxigp4_arqos"),
        saxigp4_arcache   ("saxigp4_arcache"),
        saxigp4_arburst   ("saxigp4_arburst"),
        saxigp4_arsize    ("saxigp4_arsize"),
        saxigp4_arlen     ("saxigp4_arlen"),
        saxigp4_arid      ("saxigp4_arid"),
        saxigp4_arlock    ("saxigp4_arlock"),
        saxigp4_rvalid    ("saxigp4_rvalid"),
        saxigp4_rready    ("saxigp4_rready"),
        saxigp4_rdata     ("saxigp4_rdata"),
        saxigp4_rresp     ("saxigp4_rresp"),
        saxigp4_rid       ("saxigp4_rid"),
        saxigp4_rlast     ("saxigp4_rlast"),



        saxigp5_aclk      ("saxigp5_aclk"),
        saxigp5_aresetn   ("saxigp5_aresetn"),
        saxigp5_awvalid   ("saxigp5_awvalid"),
        saxigp5_awready   ("saxigp5_awready"),
        saxigp5_awaddr    ("saxigp5_awaddr"),
        saxigp5_awprot    ("saxigp5_awprot"),
        saxigp5_awregion  ("saxigp5_awregion"),
        saxigp5_awqos     ("saxigp5_awqos"),
        saxigp5_awcache   ("saxigp5_awcache"),
        saxigp5_awburst   ("saxigp5_awburst"),
        saxigp5_awsize    ("saxigp5_awsize"),
        saxigp5_awlen     ("saxigp5_awlen"),
        saxigp5_awid      ("saxigp5_awid"),
        saxigp5_awlock    ("saxigp5_awlock"),
        saxigp5_wvalid    ("saxigp5_wvalid"),
        saxigp5_wready    ("saxigp5_wready"),
        saxigp5_wdata     ("saxigp5_wdata"),
        saxigp5_wstrb     ("saxigp5_wstrb"),
        saxigp5_wlast     ("saxigp5_wlast"),
        saxigp5_bvalid    ("saxigp5_bvalid"),
        saxigp5_bready    ("saxigp5_bready"),
        saxigp5_bresp     ("saxigp5_bresp"),
        saxigp5_bid       ("saxigp5_bid"),
        saxigp5_arvalid   ("saxigp5_arvalid"),
        saxigp5_arready   ("saxigp5_arready"),
        saxigp5_araddr    ("saxigp5_araddr"),
        saxigp5_arprot    ("saxigp5_arprot"),
        saxigp5_arregion  ("saxigp5_arregion"),
        saxigp5_arqos     ("saxigp5_arqos"),
        saxigp5_arcache   ("saxigp5_arcache"),
        saxigp5_arburst   ("saxigp5_arburst"),
        saxigp5_arsize    ("saxigp5_arsize"),
        saxigp5_arlen     ("saxigp5_arlen"),
        saxigp5_arid      ("saxigp5_arid"),
        saxigp5_arlock    ("saxigp5_arlock"),
        saxigp5_rvalid    ("saxigp5_rvalid"),
        saxigp5_rready    ("saxigp5_rready"),
        saxigp5_rdata     ("saxigp5_rdata"),
        saxigp5_rresp     ("saxigp5_rresp"),
        saxigp5_rid       ("saxigp5_rid"),
        saxigp5_rlast     ("saxigp5_rlast"),



        saxigp6_aclk      ("saxigp6_aclk"),
        saxigp6_aresetn   ("saxigp6_aresetn"),
        saxigp6_awvalid   ("saxigp6_awvalid"),
        saxigp6_awready   ("saxigp6_awready"),
        saxigp6_awaddr    ("saxigp6_awaddr"),
        saxigp6_awprot    ("saxigp6_awprot"),
        saxigp6_awregion  ("saxigp6_awregion"),
        saxigp6_awqos     ("saxigp6_awqos"),
        saxigp6_awcache   ("saxigp6_awcache"),
        saxigp6_awburst   ("saxigp6_awburst"),
        saxigp6_awsize    ("saxigp6_awsize"),
        saxigp6_awlen     ("saxigp6_awlen"),
        saxigp6_awid      ("saxigp6_awid"),
        saxigp6_awlock    ("saxigp6_awlock"),
        saxigp6_wvalid    ("saxigp6_wvalid"),
        saxigp6_wready    ("saxigp6_wready"),
        saxigp6_wdata     ("saxigp6_wdata"),
        saxigp6_wstrb     ("saxigp6_wstrb"),
        saxigp6_wlast     ("saxigp6_wlast"),
        saxigp6_bvalid    ("saxigp6_bvalid"),
        saxigp6_bready    ("saxigp6_bready"),
        saxigp6_bresp     ("saxigp6_bresp"),
        saxigp6_bid       ("saxigp6_bid"),
        saxigp6_arvalid   ("saxigp6_arvalid"),
        saxigp6_arready   ("saxigp6_arready"),
        saxigp6_araddr    ("saxigp6_araddr"),
        saxigp6_arprot    ("saxigp6_arprot"),
        saxigp6_arregion  ("saxigp6_arregion"),
        saxigp6_arqos     ("saxigp6_arqos"),
        saxigp6_arcache   ("saxigp6_arcache"),
        saxigp6_arburst   ("saxigp6_arburst"),
        saxigp6_arsize    ("saxigp6_arsize"),
        saxigp6_arlen     ("saxigp6_arlen"),
        saxigp6_arid      ("saxigp6_arid"),
        saxigp6_arlock    ("saxigp6_arlock"),
        saxigp6_rvalid    ("saxigp6_rvalid"),
        saxigp6_rready    ("saxigp6_rready"),
        saxigp6_rdata     ("saxigp6_rdata"),
        saxigp6_rresp     ("saxigp6_rresp"),
        saxigp6_rid       ("saxigp6_rid"),
        saxigp6_rlast     ("saxigp6_rlast"),


        saxiacp_aclk      ("saxiacp_aclk"),
        saxiacp_aresetn   ("saxiacp_aresetn"),
        saxiacp_awvalid   ("saxiacp_awvalid"),
        saxiacp_awready   ("saxiacp_awready"),
        saxiacp_awaddr    ("saxiacp_awaddr"),
        saxiacp_awprot    ("saxiacp_awprot"),
        saxiacp_awregion  ("saxiacp_awregion"),
        saxiacp_awqos     ("saxiacp_awqos"),
        saxiacp_awcache   ("saxiacp_awcache"),
        saxiacp_awburst   ("saxiacp_awburst"),
        saxiacp_awsize    ("saxiacp_awsize"),
        saxiacp_awlen     ("saxiacp_awlen"),
        saxiacp_awid      ("saxiacp_awid"),
        saxiacp_awlock    ("saxiacp_awlock"),
        saxiacp_wvalid    ("saxiacp_wvalid"),
        saxiacp_wready    ("saxiacp_wready"),
        saxiacp_wdata     ("saxiacp_wdata"),
        saxiacp_wstrb     ("saxiacp_wstrb"),
        saxiacp_wlast     ("saxiacp_wlast"),
        saxiacp_bvalid    ("saxiacp_bvalid"),
        saxiacp_bready    ("saxiacp_bready"),
        saxiacp_bresp     ("saxiacp_bresp"),
        saxiacp_bid       ("saxiacp_bid"),
        saxiacp_arvalid   ("saxiacp_arvalid"),
        saxiacp_arready   ("saxiacp_arready"),
        saxiacp_araddr    ("saxiacp_araddr"),
        saxiacp_arprot    ("saxiacp_arprot"),
        saxiacp_arregion  ("saxiacp_arregion"),
        saxiacp_arqos     ("saxiacp_arqos"),
        saxiacp_arcache   ("saxiacp_arcache"),
        saxiacp_arburst   ("saxiacp_arburst"),
        saxiacp_arsize    ("saxiacp_arsize"),
        saxiacp_arlen     ("saxiacp_arlen"),
        saxiacp_arid      ("saxiacp_arid"),
        saxiacp_arlock    ("saxiacp_arlock"),
        saxiacp_rvalid    ("saxiacp_rvalid"),
        saxiacp_rready    ("saxiacp_rready"),
        saxiacp_rdata     ("saxiacp_rdata"),
        saxiacp_rresp     ("saxiacp_rresp"),
        saxiacp_rid       ("saxiacp_rid"),
        saxiacp_rlast     ("saxiacp_rlast"),

        sacefpd_aclk      ("sacefpd_aclk"),
        sacefpd_aresetn   ("sacefpd_aresetn"),
        sacefpd_awvalid   ("sacefpd_awvalid"),
        sacefpd_awready   ("sacefpd_awready"),
        sacefpd_awaddr    ("sacefpd_awaddr"),
        sacefpd_awprot    ("sacefpd_awprot"),
        sacefpd_awregion  ("sacefpd_awregion"),
        sacefpd_awqos     ("sacefpd_awqos"),
        sacefpd_awcache   ("sacefpd_awcache"),
        sacefpd_awburst   ("sacefpd_awburst"),
        sacefpd_awsize    ("sacefpd_awsize"),
        sacefpd_awlen     ("sacefpd_awlen"),
        sacefpd_awid      ("sacefpd_awid"),
        sacefpd_awlock    ("sacefpd_awlock"),
        sacefpd_wvalid    ("sacefpd_wvalid"),
        sacefpd_wready    ("sacefpd_wready"),
        sacefpd_wdata     ("sacefpd_wdata"),
        sacefpd_wstrb     ("sacefpd_wstrb"),
        sacefpd_wlast     ("sacefpd_wlast"),
        sacefpd_bvalid    ("sacefpd_bvalid"),
        sacefpd_bready    ("sacefpd_bready"),
        sacefpd_bresp     ("sacefpd_bresp"),
        sacefpd_bid       ("sacefpd_bid"),
        sacefpd_arvalid   ("sacefpd_arvalid"),
        sacefpd_arready   ("sacefpd_arready"),
        sacefpd_araddr    ("sacefpd_araddr"),
        sacefpd_arprot    ("sacefpd_arprot"),
        sacefpd_arregion  ("sacefpd_arregion"),
        sacefpd_arqos     ("sacefpd_arqos"),
        sacefpd_arcache   ("sacefpd_arcache"),
        sacefpd_arburst   ("sacefpd_arburst"),
        sacefpd_arsize    ("sacefpd_arsize"),
        sacefpd_arlen     ("sacefpd_arlen"),
        sacefpd_arid      ("sacefpd_arid"),
        sacefpd_arlock    ("sacefpd_arlock"),
        sacefpd_rvalid    ("sacefpd_rvalid"),
        sacefpd_rready    ("sacefpd_rready"),
        sacefpd_rdata     ("sacefpd_rdata"),
        sacefpd_rresp     ("sacefpd_rresp"),
        sacefpd_rid       ("sacefpd_rid"),
        sacefpd_rlast     ("sacefpd_rlast"),


        pl_ps_irq0          ("pl_ps_irq0"),
        pl_ps_irq1          ("pl_ps_irq1"),
        rst_o               ("rst_o")
    {
        // QEMU related
        /////////////////


        cout << "QEMU connection path is : " << QEMU_PATH_TO_SOCKET_G << endl;
        if (QEMU_PATH_TO_SOCKET_G.length() > 107) {
            cout << "Path in QEMU_PATH_TO_SOCKET_G is too long. Max length is 107 characters. " << endl;
            cout << "QEMU_PATH_TO_SOCKET_G length: " << QEMU_PATH_TO_SOCKET_G.length() << endl;
        }
        cout << "QEMU_SYNC_QUANTUM_G " << QEMU_SYNC_QUANTUM_G << endl;

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
        register_int_parameter("M_AXI_HPM0_FPD_ENABLE_G");
        register_int_parameter("M_AXI_HPM1_FPD_ENABLE_G");
        register_int_parameter("M_AXI_HPM0_LPD_ENABLE_G");

        register_int_parameter("S_AXI_HPC0_FPD_ENABLE_G");
        register_int_parameter("S_AXI_HPC1_FPD_ENABLE_G");

        register_int_parameter("S_AXI_HP0_FPD_ENABLE_G");
        register_int_parameter("S_AXI_HP1_FPD_ENABLE_G");
        register_int_parameter("S_AXI_HP2_FPD_ENABLE_G");
        register_int_parameter("S_AXI_HP3_FPD_ENABLE_G");

        register_int_parameter("S_AXI_LPD_ENABLE_G");
        register_int_parameter("S_AXI_ACP_FPD_ENABLE_G");
        register_int_parameter("S_AXI_ACE_FPD_ENABLE_G");

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

        SC_METHOD(assign_irq0);
        sensitive << pl_ps_irq0;
        dont_initialize();

        SC_METHOD(assign_irq1);
        sensitive << pl_ps_irq1;
        dont_initialize();

        /* Tie off any remaining unconnected signals.  */

        SC_THREAD(pull_reset);
    }

private:
    tlm_utils::tlm_quantumkeeper m_qk;
};
