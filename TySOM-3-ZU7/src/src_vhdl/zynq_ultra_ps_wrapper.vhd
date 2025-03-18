-------------------------------------------------------------------------------
--
-- Copyright (c) 2020 REDS, Rick Wertenbroek <rick.wertenbroek@heig-vd.ch>
-- All rights reserved.
--
-- Redistribution and use in source and binary forms, with or without
-- modification, are permitted provided that the following conditions are met:
--
-- 1. Redistributions of source code must retain the above copyright notice,
-- this list of conditions and the following disclaimer.
--
-- 2. Redistributions in binary form must reproduce the above copyright notice,
-- this list of conditions and the following disclaimer in the documentation
-- and/or other materials provided with the distribution.
--
-- 3. Neither the name of the copyright holder nor the names of its
-- contributors may be used to endorse or promote products derived from this
-- software without specific prior written permission.
--
-- THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
-- AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
-- IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
-- ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
-- LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
-- CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
-- SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
-- INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
-- CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
-- ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
-- POSSIBILITY OF SUCH DAMAGE.
--
-------------------------------------------------------------------------------
-- File         : zynq7_ps_wrapper.vhd
-- Description  : This is a wrapper for zynq7_ps_wrapper with default values
--                for input ports (to avoid writing all the ports connections
--                if not needed, e.g., not enabled).
--
-- Author       : Rick Wertenbroek
-- Date         : 16.04.20
-- Version      : 0.0
--
-- VHDL std     : 2008
-- Dependencies :
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity zynq_ultra_ps_wrapper is
    generic(
        QEMU_PATH_TO_SOCKET_G    : string  := "unix:/path/to/qemu_cosim/qemu-rport-_cosim@0";
        QEMU_SYNC_QUANTUM_G      : integer := 1000000;
        FCLK_CLK0_PERIOD_IN_NS_G : integer := 20000;
        FCLK_CLK1_PERIOD_IN_NS_G : integer := -1;
        FCLK_CLK2_PERIOD_IN_NS_G : integer := -1;
        FCLK_CLK3_PERIOD_IN_NS_G : integer := -1;
        M_AXI_HPM0_FPD_ENABLE_G  : integer := 0;
        M_AXI_HPM1_FPD_ENABLE_G  : integer := 0;
        M_AXI_HPM0_LPD_ENABLE_G  : integer := 0;
        --
        S_AXI_HPC0_FPD_ENABLE_G  : integer := 0;
        S_AXI_HPC1_FPD_ENABLE_G  : integer := 0;

        S_AXI_HP0_FPD_ENABLE_G   : integer := 0;
        S_AXI_HP1_FPD_ENABLE_G   : integer := 0;
        S_AXI_HP2_FPD_ENABLE_G   : integer := 0;
        S_AXI_HP3_FPD_ENABLE_G   : integer := 0;
        S_AXI_LPD_ENABLE_G       : integer := 0;
        S_AXI_ACP_FPD_ENABLE_G   : integer := 0;
        S_AXI_ACE_FPD_ENABLE_G   : integer := 0;
        --
        -- If the generics below change values, the constants in zynq_ps.h must
        -- be changed accordingly (because SystemC (C++) templates cannot take
        -- generic values and constants must be known at compilation time)
        --
        M_AXI_HPM0_FPD_ADDR_WIDTH_G   : integer := 40;
        M_AXI_HPM0_FPD_DATA_WIDTH_G   : integer := 128;
        M_AXI_HPM0_FPD_ID_WIDTH_G     : integer := 16;
        M_AXI_HPM0_FPD_AXLEN_WIDTH_G  : integer := 8;
        M_AXI_HPM0_FPD_AXLOCK_WIDTH_G : integer := 1;
        --
        M_AXI_HPM1_FPD_ADDR_WIDTH_G   : integer := 40;
        M_AXI_HPM1_FPD_DATA_WIDTH_G   : integer := 128;
        M_AXI_HPM1_FPD_ID_WIDTH_G     : integer := 16;
        M_AXI_HPM1_FPD_AXLEN_WIDTH_G  : integer := 8;
        M_AXI_HPM1_FPD_AXLOCK_WIDTH_G : integer := 1;
        --
        M_AXI_HPM0_LPD_ADDR_WIDTH_G   : integer := 40;
        M_AXI_HPM0_LPD_DATA_WIDTH_G   : integer := 128;
        M_AXI_HPM0_LPD_ID_WIDTH_G     : integer := 16;
        M_AXI_HPM0_LPD_AXLEN_WIDTH_G  : integer := 8;
        M_AXI_HPM0_LPD_AXLOCK_WIDTH_G : integer := 1;
        --
        S_AXI_HPC0_FPD_ADDR_WIDTH_G   : integer := 49;
        S_AXI_HPC0_FPD_DATA_WIDTH_G   : integer := 128;
        S_AXI_HPC0_FPD_ID_WIDTH_G     : integer := 16;
        S_AXI_HPC0_FPD_AXLEN_WIDTH_G  : integer := 8;
        S_AXI_HPC0_FPD_AXLOCK_WIDTH_G : integer := 1;
        --
        S_AXI_HPC1_FPD_ADDR_WIDTH_G   : integer := 49;
        S_AXI_HPC1_FPD_DATA_WIDTH_G   : integer := 128;
        S_AXI_HPC1_FPD_ID_WIDTH_G     : integer := 16;
        S_AXI_HPC1_FPD_AXLEN_WIDTH_G  : integer := 8;
        S_AXI_HPC1_FPD_AXLOCK_WIDTH_G : integer := 1;
        --
        S_AXI_HP0_FPD_ADDR_WIDTH_G   : integer := 49;
        S_AXI_HP0_FPD_DATA_WIDTH_G   : integer := 128;
        S_AXI_HP0_FPD_ID_WIDTH_G     : integer := 16;
        S_AXI_HP0_FPD_AXLEN_WIDTH_G  : integer := 8;
        S_AXI_HP0_FPD_AXLOCK_WIDTH_G : integer := 1;
        --
        S_AXI_HP1_FPD_ADDR_WIDTH_G   : integer := 40;
        S_AXI_HP1_FPD_DATA_WIDTH_G   : integer := 128;
        S_AXI_HP1_FPD_ID_WIDTH_G     : integer := 16;
        S_AXI_HP1_FPD_AXLEN_WIDTH_G  : integer := 8;
        S_AXI_HP1_FPD_AXLOCK_WIDTH_G : integer := 1;
        --
        S_AXI_HP2_FPD_ADDR_WIDTH_G   : integer := 49;
        S_AXI_HP2_FPD_DATA_WIDTH_G   : integer := 128;
        S_AXI_HP2_FPD_ID_WIDTH_G     : integer := 16;
        S_AXI_HP2_FPD_AXLEN_WIDTH_G  : integer := 8;
        S_AXI_HP2_FPD_AXLOCK_WIDTH_G : integer := 1;
        --
        S_AXI_HP3_FPD_ADDR_WIDTH_G   : integer := 49;
        S_AXI_HP3_FPD_DATA_WIDTH_G   : integer := 128;
        S_AXI_HP3_FPD_ID_WIDTH_G     : integer := 16;
        S_AXI_HP3_FPD_AXLEN_WIDTH_G  : integer := 8;
        S_AXI_HP3_FPD_AXLOCK_WIDTH_G : integer := 1;

        S_AXI_LPD_ADDR_WIDTH_G       : integer := 49;
        S_AXI_LPD_DATA_WIDTH_G       : integer := 128;
        S_AXI_LPD_ID_WIDTH_G         : integer := 16;
        S_AXI_LPD_AXLEN_WIDTH_G      : integer := 8;
        S_AXI_LPD_AXLOCK_WIDTH_G     : integer := 1;

        S_AXI_ACP_FPD_ADDR_WIDTH_G       : integer := 40;
        S_AXI_ACP_FPD_DATA_WIDTH_G       : integer := 128;
        S_AXI_ACP_FPD_ID_WIDTH_G         : integer := 16;
        S_AXI_ACP_FPD_AXLEN_WIDTH_G      : integer := 8;
        S_AXI_ACP_FPD_AXLOCK_WIDTH_G     : integer := 1;

        S_AXI_ACE_FPD_ADDR_WIDTH_G       : integer := 44;
        S_AXI_ACE_FPD_DATA_WIDTH_G       : integer := 128;
        S_AXI_ACE_FPD_ID_WIDTH_G         : integer := 16;
        S_AXI_ACE_FPD_AXLEN_WIDTH_G      : integer := 8;
        S_AXI_ACE_FPD_AXLOCK_WIDTH_G     : integer := 1
        );

    port(
        -- Clocks
        pl_clk0         : out std_logic;
        pl_clk1         : out std_logic;
        pl_clk2         : out std_logic;
        pl_clk3         : out std_logic;
        -- M AXI GP0
        maxihpm0_fpd_aclk    : in  std_logic := '0';
        maxigp0_aresetn : in  std_logic := '0';
        maxigp0_awvalid : out std_logic;
        maxigp0_awready : in  std_logic := '0';
        maxigp0_awaddr  : out std_logic_vector(M_AXI_HPM0_FPD_ADDR_WIDTH_G-1 downto 0);
        maxigp0_awprot  : out std_logic_vector(2 downto 0);
        maxigp0_awqos   : out std_logic_vector(3 downto 0);
        maxigp0_awcache : out std_logic_vector(3 downto 0);
        maxigp0_awburst : out std_logic_vector(1 downto 0);
        maxigp0_awsize  : out std_logic_vector(2 downto 0);
        maxigp0_awlen   : out std_logic_vector(M_AXI_HPM0_FPD_AXLEN_WIDTH_G-1 downto 0);
        maxigp0_awid    : out std_logic_vector(M_AXI_HPM0_FPD_ID_WIDTH_G-1 downto 0);
        --maxigp0_awlock  : out std_logic_vector(M_AXI_HPM0_FPD_AXLOCK_WIDTH_G-1 downto 0);
        maxigp0_awlock  : out std_logic;
        maxigp0_wid     : out std_logic_vector(M_AXI_HPM0_FPD_ID_WIDTH_G-1 downto 0);
        maxigp0_wvalid  : out std_logic;
        maxigp0_wready  : in  std_logic := '0';
        maxigp0_wdata   : out std_logic_vector(M_AXI_HPM0_FPD_DATA_WIDTH_G-1 downto 0);
        maxigp0_wstrb   : out std_logic_vector(M_AXI_HPM0_FPD_DATA_WIDTH_G/8-1 downto 0);
        maxigp0_wlast   : out std_logic;
        maxigp0_bvalid  : in  std_logic := '0';
        maxigp0_bready  : out std_logic;
        maxigp0_bresp   : in  std_logic_vector(1 downto 0) := (others => '0');
        maxigp0_bid     : in  std_logic_vector(M_AXI_HPM0_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        maxigp0_arvalid : out std_logic;
        maxigp0_arready : in  std_logic := '0';
        maxigp0_araddr  : out std_logic_vector(M_AXI_HPM0_FPD_ADDR_WIDTH_G-1 downto 0);
        maxigp0_arprot  : out std_logic_vector(2 downto 0);
        maxigp0_arqos   : out std_logic_vector(3 downto 0);
        maxigp0_arcache : out std_logic_vector(3 downto 0);
        maxigp0_arburst : out std_logic_vector(1 downto 0);
        maxigp0_arsize  : out std_logic_vector(2 downto 0);
        maxigp0_arlen   : out std_logic_vector(M_AXI_HPM0_FPD_AXLEN_WIDTH_G-1 downto 0);
        maxigp0_arid    : out std_logic_vector(M_AXI_HPM0_FPD_ID_WIDTH_G-1 downto 0);
        --maxigp0_arlock  : out std_logic_vector(M_AXI_HPM0_FPD_AXLOCK_WIDTH_G-1 downto 0);
        maxigp0_arlock  : out std_logic;
        maxigp0_rvalid  : in  std_logic := '0';
        maxigp0_rready  : out std_logic;
        maxigp0_rdata   : in  std_logic_vector(M_AXI_HPM0_FPD_DATA_WIDTH_G-1 downto 0) := (others => '0');
        maxigp0_rresp   : in  std_logic_vector(1 downto 0) := (others => '0');
        maxigp0_rid     : in  std_logic_vector(M_AXI_HPM0_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        maxigp0_rlast   : in  std_logic := '0';
        -- M AXI GP1
        maxihpm1_fpd_aclk    : in  std_logic := '0';
        maxigp1_aresetn : in  std_logic := '0';
        maxigp1_awvalid : out std_logic;
        maxigp1_awready : in  std_logic := '0';
        maxigp1_awaddr  : out std_logic_vector(M_AXI_HPM1_FPD_ADDR_WIDTH_G-1 downto 0);
        maxigp1_awprot  : out std_logic_vector(2 downto 0);
        maxigp1_awqos   : out std_logic_vector(3 downto 0);
        maxigp1_awcache : out std_logic_vector(3 downto 0);
        maxigp1_awburst : out std_logic_vector(1 downto 0);
        maxigp1_awsize  : out std_logic_vector(2 downto 0);
        maxigp1_awlen   : out std_logic_vector(M_AXI_HPM1_FPD_AXLEN_WIDTH_G-1 downto 0);
        maxigp1_awid    : out std_logic_vector(M_AXI_HPM1_FPD_ID_WIDTH_G-1 downto 0);
        --maxigp1_awlock  : out std_logic_vector(M_AXI_HPM1_FPD_AXLOCK_WIDTH_G-1 downto 0);
        maxigp1_awlock  : out std_logic;
        maxigp1_wid     : out std_logic_vector(M_AXI_HPM1_FPD_ID_WIDTH_G-1 downto 0);
        maxigp1_wvalid  : out std_logic;
        maxigp1_wready  : in  std_logic := '0';
        maxigp1_wdata   : out std_logic_vector(M_AXI_HPM1_FPD_DATA_WIDTH_G-1 downto 0);
        maxigp1_wstrb   : out std_logic_vector(M_AXI_HPM1_FPD_DATA_WIDTH_G/8-1 downto 0);
        maxigp1_wlast   : out std_logic;
        maxigp1_bvalid  : in  std_logic := '0';
        maxigp1_bready  : out std_logic;
        maxigp1_bresp   : in  std_logic_vector(1 downto 0) := (others => '0');
        maxigp1_bid     : in  std_logic_vector(M_AXI_HPM1_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        maxigp1_arvalid : out std_logic;
        maxigp1_arready : in  std_logic := '0';
        maxigp1_araddr  : out std_logic_vector(M_AXI_HPM1_FPD_ADDR_WIDTH_G-1 downto 0);
        maxigp1_arprot  : out std_logic_vector(2 downto 0);
        maxigp1_arqos   : out std_logic_vector(3 downto 0);
        maxigp1_arcache : out std_logic_vector(3 downto 0);
        maxigp1_arburst : out std_logic_vector(1 downto 0);
        maxigp1_arsize  : out std_logic_vector(2 downto 0);
        maxigp1_arlen   : out std_logic_vector(M_AXI_HPM1_FPD_AXLEN_WIDTH_G-1 downto 0);
        maxigp1_arid    : out std_logic_vector(M_AXI_HPM1_FPD_ID_WIDTH_G-1 downto 0);
        --maxigp1_arlock  : out std_logic_vector(M_AXI_HPM1_FPD_AXLOCK_WIDTH_G-1 downto 0);
        maxigp1_arlock  : out std_logic;
        maxigp1_rvalid  : in  std_logic := '0';
        maxigp1_rready  : out std_logic;
        maxigp1_rdata   : in  std_logic_vector(M_AXI_HPM1_FPD_DATA_WIDTH_G-1 downto 0) := (others => '0');
        maxigp1_rresp   : in  std_logic_vector(1 downto 0) := (others => '0');
        maxigp1_rid     : in  std_logic_vector(M_AXI_HPM1_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        maxigp1_rlast   : in  std_logic := '0';

        -- M AXI GP2
        maxihpm0_lpd_aclk    : in  std_logic := '0';
        maxigp2_aresetn : in  std_logic := '0';
        maxigp2_awvalid : out std_logic;
        maxigp2_awready : in  std_logic := '0';
        maxigp2_awaddr  : out std_logic_vector(M_AXI_HPM0_LPD_ADDR_WIDTH_G-1 downto 0);
        maxigp2_awprot  : out std_logic_vector(2 downto 0);
        maxigp2_awqos   : out std_logic_vector(3 downto 0);
        maxigp2_awcache : out std_logic_vector(3 downto 0);
        maxigp2_awburst : out std_logic_vector(1 downto 0);
        maxigp2_awsize  : out std_logic_vector(2 downto 0);
        maxigp2_awlen   : out std_logic_vector(M_AXI_HPM0_LPD_AXLEN_WIDTH_G-1 downto 0);
        maxigp2_awid    : out std_logic_vector(M_AXI_HPM0_LPD_ID_WIDTH_G-1 downto 0);
        --maxigp2_awlock  : out std_logic_vector(M_AXI_HPM0_LPD_AXLOCK_WIDTH_G-1 downto 0);
        maxigp2_awlock  : out std_logic;
        maxigp2_wid     : out std_logic_vector(M_AXI_HPM0_LPD_ID_WIDTH_G-1 downto 0);
        maxigp2_wvalid  : out std_logic;
        maxigp2_wready  : in  std_logic := '0';
        maxigp2_wdata   : out std_logic_vector(M_AXI_HPM0_LPD_DATA_WIDTH_G-1 downto 0);
        maxigp2_wstrb   : out std_logic_vector(M_AXI_HPM0_LPD_DATA_WIDTH_G/8-1 downto 0);
        maxigp2_wlast   : out std_logic;
        maxigp2_bvalid  : in  std_logic := '0';
        maxigp2_bready  : out std_logic;
        maxigp2_bresp   : in  std_logic_vector(1 downto 0) := (others => '0');
        maxigp2_bid     : in  std_logic_vector(M_AXI_HPM0_LPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        maxigp2_arvalid : out std_logic;
        maxigp2_arready : in  std_logic := '0';
        maxigp2_araddr  : out std_logic_vector(M_AXI_HPM0_LPD_ADDR_WIDTH_G-1 downto 0);
        maxigp2_arprot  : out std_logic_vector(2 downto 0);
        maxigp2_arqos   : out std_logic_vector(3 downto 0);
        maxigp2_arcache : out std_logic_vector(3 downto 0);
        maxigp2_arburst : out std_logic_vector(1 downto 0);
        maxigp2_arsize  : out std_logic_vector(2 downto 0);
        maxigp2_arlen   : out std_logic_vector(M_AXI_HPM0_LPD_AXLEN_WIDTH_G-1 downto 0);
        maxigp2_arid    : out std_logic_vector(M_AXI_HPM0_LPD_ID_WIDTH_G-1 downto 0);
        --maxigp2_arlock  : out std_logic_vector(M_AXI_HPM0_LPD_AXLOCK_WIDTH_G-1 downto 0);
        maxigp2_arlock  : out std_logic;
        maxigp2_rvalid  : in  std_logic := '0';
        maxigp2_rready  : out std_logic;
        maxigp2_rdata   : in  std_logic_vector(M_AXI_HPM0_LPD_DATA_WIDTH_G-1 downto 0) := (others => '0');
        maxigp2_rresp   : in  std_logic_vector(1 downto 0) := (others => '0');
        maxigp2_rid     : in  std_logic_vector(M_AXI_HPM0_LPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        maxigp2_rlast   : in  std_logic := '0';
        -- S AXI HPC0
        saxihpc0_fpd_aclk    : in  std_logic := '0';
        saxigp0_aresetn : in  std_logic := '0';
        saxigp0_awvalid : in  std_logic := '0';
        saxigp0_awready : out std_logic;
        saxigp0_awaddr  : in  std_logic_vector(S_AXI_HPC0_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp0_awprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp0_awqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp0_awcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp0_awburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp0_awsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp0_awlen   : in  std_logic_vector(S_AXI_HPC0_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp0_awid    : in  std_logic_vector(S_AXI_HPC0_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp0_awlock  : in  std_logic_vector(S_AXI_HPC0_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp0_awlock  : in  std_logic := '0';
        saxigp0_wid     : in  std_logic_vector(S_AXI_HPC0_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');  -- Unconnected and unused
        saxigp0_wvalid  : in  std_logic := '0';
        saxigp0_wready  : out std_logic;
        saxigp0_wdata   : in  std_logic_vector(S_AXI_HPC0_FPD_DATA_WIDTH_G-1 downto 0) := (others => '0');
        saxigp0_wstrb   : in  std_logic_vector(S_AXI_HPC0_FPD_DATA_WIDTH_G/8-1 downto 0) := (others => '0');
        saxigp0_wlast   : in  std_logic := '0';
        saxigp0_bvalid  : out std_logic;
        saxigp0_bready  : in  std_logic := '0';
        saxigp0_bresp   : out std_logic_vector(1 downto 0);
        saxigp0_bid     : out std_logic_vector(S_AXI_HPC0_FPD_ID_WIDTH_G-1 downto 0);
        saxigp0_arvalid : in  std_logic := '0';
        saxigp0_arready : out std_logic;
        saxigp0_araddr  : in  std_logic_vector(S_AXI_HPC0_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp0_arprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp0_arqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp0_arcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp0_arburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp0_arsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp0_arlen   : in  std_logic_vector(S_AXI_HPC0_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp0_arid    : in  std_logic_vector(S_AXI_HPC0_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp0_arlock  : in  std_logic_vector(S_AXI_HPC0_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp0_arlock  : in  std_logic := '0';
        saxigp0_rvalid  : out std_logic;
        saxigp0_rready  : in  std_logic := '0';
        saxigp0_rdata   : out std_logic_vector(S_AXI_HPC0_FPD_DATA_WIDTH_G-1 downto 0);
        saxigp0_rresp   : out std_logic_vector(1 downto 0);
        saxigp0_rid     : out std_logic_vector(S_AXI_HPC0_FPD_ID_WIDTH_G-1 downto 0);
        saxigp0_rlast   : out std_logic;
        -- S AXI HPC1
        saxihpc1_fpd_aclk    : in  std_logic := '0';
        saxigp1_aresetn : in  std_logic := '0';
        saxigp1_awvalid : in  std_logic := '0';
        saxigp1_awready : out std_logic;
        saxigp1_awaddr  : in  std_logic_vector(S_AXI_HPC1_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp1_awprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp1_awqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp1_awcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp1_awburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp1_awsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp1_awlen   : in  std_logic_vector(S_AXI_HPC1_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp1_awid    : in  std_logic_vector(S_AXI_HPC1_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp1_awlock  : in  std_logic_vector(S_AXI_HPC1_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp1_awlock  : in  std_logic := '0';
        saxigp1_wid     : in  std_logic_vector(S_AXI_HPC1_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');  -- Unconnected and unused
        saxigp1_wvalid  : in  std_logic := '0';
        saxigp1_wready  : out std_logic;
        saxigp1_wdata   : in  std_logic_vector(S_AXI_HPC1_FPD_DATA_WIDTH_G-1 downto 0) := (others => '0');
        saxigp1_wstrb   : in  std_logic_vector(S_AXI_HPC1_FPD_DATA_WIDTH_G/8-1 downto 0) := (others => '0');
        saxigp1_wlast   : in  std_logic := '0';
        saxigp1_bvalid  : out std_logic;
        saxigp1_bready  : in  std_logic := '0';
        saxigp1_bresp   : out std_logic_vector(1 downto 0);
        saxigp1_bid     : out std_logic_vector(S_AXI_HPC1_FPD_ID_WIDTH_G-1 downto 0);
        saxigp1_arvalid : in  std_logic := '0';
        saxigp1_arready : out std_logic;
        saxigp1_araddr  : in  std_logic_vector(S_AXI_HPC1_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp1_arprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp1_arqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp1_arcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp1_arburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp1_arsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp1_arlen   : in  std_logic_vector(S_AXI_HPC1_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp1_arid    : in  std_logic_vector(S_AXI_HPC1_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp1_arlock  : in  std_logic_vector(S_AXI_HPC1_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp1_arlock  : in  std_logic := '0';
        saxigp1_rvalid  : out std_logic;
        saxigp1_rready  : in  std_logic := '0';
        saxigp1_rdata   : out std_logic_vector(S_AXI_HPC1_FPD_DATA_WIDTH_G-1 downto 0);
        saxigp1_rresp   : out std_logic_vector(1 downto 0);
        saxigp1_rid     : out std_logic_vector(S_AXI_HPC1_FPD_ID_WIDTH_G-1 downto 0);
        saxigp1_rlast   : out std_logic;
        -- S AXI HP0
        saxihp0_fpd_aclk    : in  std_logic := '0';
        saxigp2_aresetn : in  std_logic := '0';
        saxigp2_awvalid : in  std_logic := '0';
        saxigp2_awready : out std_logic;
        saxigp2_awaddr  : in  std_logic_vector(S_AXI_HP0_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp2_awprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp2_awqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp2_awcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp2_awburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp2_awsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp2_awlen   : in  std_logic_vector(S_AXI_HP0_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp2_awid    : in  std_logic_vector(S_AXI_HP0_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp2_awlock  : in  std_logic_vector(S_AXI_HP0_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp2_awlock  : in  std_logic := '0';
        saxigp2_wid     : in  std_logic_vector(S_AXI_HP0_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');  -- Unconnected and unused
        saxigp2_wvalid  : in  std_logic := '0';
        saxigp2_wready  : out std_logic;
        saxigp2_wdata   : in  std_logic_vector(S_AXI_HP0_FPD_DATA_WIDTH_G-1 downto 0) := (others => '0');
        saxigp2_wstrb   : in  std_logic_vector(S_AXI_HP0_FPD_DATA_WIDTH_G/8-1 downto 0) := (others => '0');
        saxigp2_wlast   : in  std_logic := '0';
        saxigp2_bvalid  : out std_logic;
        saxigp2_bready  : in  std_logic := '0';
        saxigp2_bresp   : out std_logic_vector(1 downto 0);
        saxigp2_bid     : out std_logic_vector(S_AXI_HP0_FPD_ID_WIDTH_G-1 downto 0);
        saxigp2_arvalid : in  std_logic := '0';
        saxigp2_arready : out std_logic;
        saxigp2_araddr  : in  std_logic_vector(S_AXI_HP0_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp2_arprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp2_arqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp2_arcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp2_arburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp2_arsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp2_arlen   : in  std_logic_vector(S_AXI_HP0_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp2_arid    : in  std_logic_vector(S_AXI_HP0_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp2_arlock  : in  std_logic_vector(S_AXI_HP0_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp2_arlock  : in  std_logic := '0';
        saxigp2_rvalid  : out std_logic;
        saxigp2_rready  : in  std_logic := '0';
        saxigp2_rdata   : out std_logic_vector(S_AXI_HP0_FPD_DATA_WIDTH_G-1 downto 0);
        saxigp2_rresp   : out std_logic_vector(1 downto 0);
        saxigp2_rid     : out std_logic_vector(S_AXI_HP0_FPD_ID_WIDTH_G-1 downto 0);
        saxigp2_rlast   : out std_logic;
        -- S AXI HP1
        saxihp1_fpd_aclk    : in  std_logic := '0';
        saxigp3_aresetn : in  std_logic := '0';
        saxigp3_awvalid : in  std_logic := '0';
        saxigp3_awready : out std_logic;
        saxigp3_awaddr  : in  std_logic_vector(S_AXI_HP1_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp3_awprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp3_awqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp3_awcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp3_awburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp3_awsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp3_awlen   : in  std_logic_vector(S_AXI_HP1_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp3_awid    : in  std_logic_vector(S_AXI_HP1_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp3_awlock  : in  std_logic_vector(S_AXI_HP1_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp3_awlock  : in  std_logic := '0';
        saxigp3_wid     : in  std_logic_vector(S_AXI_HP1_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');  -- Unconnected and unused
        saxigp3_wvalid  : in  std_logic := '0';
        saxigp3_wready  : out std_logic;
        saxigp3_wdata   : in  std_logic_vector(S_AXI_HP1_FPD_DATA_WIDTH_G-1 downto 0) := (others => '0');
        saxigp3_wstrb   : in  std_logic_vector(S_AXI_HP1_FPD_DATA_WIDTH_G/8-1 downto 0) := (others => '0');
        saxigp3_wlast   : in  std_logic := '0';
        saxigp3_bvalid  : out std_logic;
        saxigp3_bready  : in  std_logic := '0';
        saxigp3_bresp   : out std_logic_vector(1 downto 0);
        saxigp3_bid     : out std_logic_vector(S_AXI_HP1_FPD_ID_WIDTH_G-1 downto 0);
        saxigp3_arvalid : in  std_logic := '0';
        saxigp3_arready : out std_logic;
        saxigp3_araddr  : in  std_logic_vector(S_AXI_HP1_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp3_arprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp3_arqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp3_arcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp3_arburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp3_arsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp3_arlen   : in  std_logic_vector(S_AXI_HP1_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp3_arid    : in  std_logic_vector(S_AXI_HP1_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp3_arlock  : in  std_logic_vector(S_AXI_HP1_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp3_arlock  : in  std_logic := '0';
        saxigp3_rvalid  : out std_logic;
        saxigp3_rready  : in  std_logic := '0';
        saxigp3_rdata   : out std_logic_vector(S_AXI_HP1_FPD_DATA_WIDTH_G-1 downto 0);
        saxigp3_rresp   : out std_logic_vector(1 downto 0);
        saxigp3_rid     : out std_logic_vector(S_AXI_HP1_FPD_ID_WIDTH_G-1 downto 0);
        saxigp3_rlast   : out std_logic;
        -- S AXI HP2
        saxihp2_fpd_aclk    : in  std_logic := '0';
        saxigp4_aresetn : in  std_logic := '0';
        saxigp4_awvalid : in  std_logic := '0';
        saxigp4_awready : out std_logic;
        saxigp4_awaddr  : in  std_logic_vector(S_AXI_HP2_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp4_awprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp4_awqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp4_awcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp4_awburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp4_awsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp4_awlen   : in  std_logic_vector(S_AXI_HP2_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp4_awid    : in  std_logic_vector(S_AXI_HP2_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp4_awlock  : in  std_logic_vector(S_AXI_HP2_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp4_awlock  : in  std_logic := '0';
        saxigp4_wid     : in  std_logic_vector(S_AXI_HP2_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');  -- Unconnected and unused
        saxigp4_wvalid  : in  std_logic := '0';
        saxigp4_wready  : out std_logic;
        saxigp4_wdata   : in  std_logic_vector(S_AXI_HP2_FPD_DATA_WIDTH_G-1 downto 0) := (others => '0');
        saxigp4_wstrb   : in  std_logic_vector(S_AXI_HP2_FPD_DATA_WIDTH_G/8-1 downto 0) := (others => '0');
        saxigp4_wlast   : in  std_logic := '0';
        saxigp4_bvalid  : out std_logic;
        saxigp4_bready  : in  std_logic := '0';
        saxigp4_bresp   : out std_logic_vector(1 downto 0);
        saxigp4_bid     : out std_logic_vector(S_AXI_HP2_FPD_ID_WIDTH_G-1 downto 0);
        saxigp4_arvalid : in  std_logic := '0';
        saxigp4_arready : out std_logic;
        saxigp4_araddr  : in  std_logic_vector(S_AXI_HP2_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp4_arprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp4_arqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp4_arcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp4_arburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp4_arsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp4_arlen   : in  std_logic_vector(S_AXI_HP2_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp4_arid    : in  std_logic_vector(S_AXI_HP2_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp4_arlock  : in  std_logic_vector(S_AXI_HP2_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp4_arlock  : in  std_logic := '0';
        saxigp4_rvalid  : out std_logic;
        saxigp4_rready  : in  std_logic := '0';
        saxigp4_rdata   : out std_logic_vector(S_AXI_HP2_FPD_DATA_WIDTH_G-1 downto 0);
        saxigp4_rresp   : out std_logic_vector(1 downto 0);
        saxigp4_rid     : out std_logic_vector(S_AXI_HP2_FPD_ID_WIDTH_G-1 downto 0);
        saxigp4_rlast   : out std_logic;
        -- S AXI HP3
        saxihp3_fpd_aclk    : in  std_logic := '0';
        saxigp5_aresetn : in  std_logic := '0';
        saxigp5_awvalid : in  std_logic := '0';
        saxigp5_awready : out std_logic;
        saxigp5_awaddr  : in  std_logic_vector(S_AXI_HP3_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp5_awprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp5_awqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp5_awcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp5_awburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp5_awsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp5_awlen   : in  std_logic_vector(S_AXI_HP3_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp5_awid    : in  std_logic_vector(S_AXI_HP3_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp5_awlock  : in  std_logic_vector(S_AXI_HP3_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp5_awlock  : in  std_logic := '0';
        saxigp5_wid     : in  std_logic_vector(S_AXI_HP3_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');  -- Unconnected and unused
        saxigp5_wvalid  : in  std_logic := '0';
        saxigp5_wready  : out std_logic;
        saxigp5_wdata   : in  std_logic_vector(S_AXI_HP3_FPD_DATA_WIDTH_G-1 downto 0) := (others => '0');
        saxigp5_wstrb   : in  std_logic_vector(S_AXI_HP3_FPD_DATA_WIDTH_G/8-1 downto 0) := (others => '0');
        saxigp5_wlast   : in  std_logic := '0';
        saxigp5_bvalid  : out std_logic;
        saxigp5_bready  : in  std_logic := '0';
        saxigp5_bresp   : out std_logic_vector(1 downto 0);
        saxigp5_bid     : out std_logic_vector(S_AXI_HP3_FPD_ID_WIDTH_G-1 downto 0);
        saxigp5_arvalid : in  std_logic := '0';
        saxigp5_arready : out std_logic;
        saxigp5_araddr  : in  std_logic_vector(S_AXI_HP3_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp5_arprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp5_arqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp5_arcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp5_arburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp5_arsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp5_arlen   : in  std_logic_vector(S_AXI_HP3_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp5_arid    : in  std_logic_vector(S_AXI_HP3_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp5_arlock  : in  std_logic_vector(S_AXI_HP3_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp5_arlock  : in  std_logic := '0';
        saxigp5_rvalid  : out std_logic;
        saxigp5_rready  : in  std_logic := '0';
        saxigp5_rdata   : out std_logic_vector(S_AXI_HP3_FPD_DATA_WIDTH_G-1 downto 0);
        saxigp5_rresp   : out std_logic_vector(1 downto 0);
        saxigp5_rid     : out std_logic_vector(S_AXI_HP3_FPD_ID_WIDTH_G-1 downto 0);
        saxigp5_rlast   : out std_logic;

        -- S AXI LPD
        saxi_lpd_aclk    : in  std_logic := '0';
        saxigp6_aresetn : in  std_logic := '0';
        saxigp6_awvalid : in  std_logic := '0';
        saxigp6_awready : out std_logic;
        saxigp6_awaddr  : in  std_logic_vector(S_AXI_LPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp6_awprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp6_awqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp6_awcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp6_awburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp6_awsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp6_awlen   : in  std_logic_vector(S_AXI_LPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp6_awid    : in  std_logic_vector(S_AXI_LPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp6_awlock  : in  std_logic_vector(S_AXI_LPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp6_awlock  : in  std_logic := '0';
        saxigp6_wid     : in  std_logic_vector(S_AXI_LPD_ID_WIDTH_G-1 downto 0) := (others => '0');  -- Unconnected and unused
        saxigp6_wvalid  : in  std_logic := '0';
        saxigp6_wready  : out std_logic;
        saxigp6_wdata   : in  std_logic_vector(S_AXI_LPD_DATA_WIDTH_G-1 downto 0) := (others => '0');
        saxigp6_wstrb   : in  std_logic_vector(S_AXI_LPD_DATA_WIDTH_G/8-1 downto 0) := (others => '0');
        saxigp6_wlast   : in  std_logic := '0';
        saxigp6_bvalid  : out std_logic;
        saxigp6_bready  : in  std_logic := '0';
        saxigp6_bresp   : out std_logic_vector(1 downto 0);
        saxigp6_bid     : out std_logic_vector(S_AXI_LPD_ID_WIDTH_G-1 downto 0);
        saxigp6_arvalid : in  std_logic := '0';
        saxigp6_arready : out std_logic;
        saxigp6_araddr  : in  std_logic_vector(S_AXI_LPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxigp6_arprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp6_arqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp6_arcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxigp6_arburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxigp6_arsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxigp6_arlen   : in  std_logic_vector(S_AXI_LPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxigp6_arid    : in  std_logic_vector(S_AXI_LPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxigp6_arlock  : in  std_logic_vector(S_AXI_LPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxigp6_arlock  : in  std_logic := '0';
        saxigp6_rvalid  : out std_logic;
        saxigp6_rready  : in  std_logic := '0';
        saxigp6_rdata   : out std_logic_vector(S_AXI_LPD_DATA_WIDTH_G-1 downto 0);
        saxigp6_rresp   : out std_logic_vector(1 downto 0);
        saxigp6_rid     : out std_logic_vector(S_AXI_LPD_ID_WIDTH_G-1 downto 0);
        saxigp6_rlast   : out std_logic;



        -- S AXI ACP
        saxiacp_fpd_aclk    : in  std_logic := '0';
        saxiacp_aresetn : in  std_logic := '0';
        saxiacp_awvalid : in  std_logic := '0';
        saxiacp_awready : out std_logic;
        saxiacp_awaddr  : in  std_logic_vector(S_AXI_ACP_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxiacp_awprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxiacp_awqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxiacp_awcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxiacp_awburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxiacp_awsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxiacp_awlen   : in  std_logic_vector(S_AXI_ACP_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxiacp_awid    : in  std_logic_vector(S_AXI_ACP_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxiacp_awlock  : in  std_logic_vector(S_AXI_ACP_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxiacp_awlock  : in  std_logic := '0';
        saxiacp_wid     : in  std_logic_vector(S_AXI_ACP_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');  -- Unconnected and unused
        saxiacp_wvalid  : in  std_logic := '0';
        saxiacp_wready  : out std_logic;
        saxiacp_wdata   : in  std_logic_vector(S_AXI_ACP_FPD_DATA_WIDTH_G-1 downto 0) := (others => '0');
        saxiacp_wstrb   : in  std_logic_vector(S_AXI_ACP_FPD_DATA_WIDTH_G/8-1 downto 0) := (others => '0');
        saxiacp_wlast   : in  std_logic := '0';
        saxiacp_bvalid  : out std_logic;
        saxiacp_bready  : in  std_logic := '0';
        saxiacp_bresp   : out std_logic_vector(1 downto 0);
        saxiacp_bid     : out std_logic_vector(S_AXI_ACP_FPD_ID_WIDTH_G-1 downto 0);
        saxiacp_arvalid : in  std_logic := '0';
        saxiacp_arready : out std_logic;
        saxiacp_araddr  : in  std_logic_vector(S_AXI_ACP_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        saxiacp_arprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxiacp_arqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        saxiacp_arcache : in  std_logic_vector(3 downto 0) := (others => '0');
        saxiacp_arburst : in  std_logic_vector(1 downto 0) := (others => '0');
        saxiacp_arsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        saxiacp_arlen   : in  std_logic_vector(S_AXI_ACP_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        saxiacp_arid    : in  std_logic_vector(S_AXI_ACP_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --saxiacp_arlock  : in  std_logic_vector(S_AXI_ACP_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        saxiacp_arlock  : in  std_logic := '0';
        saxiacp_rvalid  : out std_logic;
        saxiacp_rready  : in  std_logic := '0';
        saxiacp_rdata   : out std_logic_vector(S_AXI_ACP_FPD_DATA_WIDTH_G-1 downto 0);
        saxiacp_rresp   : out std_logic_vector(1 downto 0);
        saxiacp_rid     : out std_logic_vector(S_AXI_ACP_FPD_ID_WIDTH_G-1 downto 0);
        saxiacp_rlast   : out std_logic;


        -- S AXI ACE
        sacefpd_aclk    : in  std_logic := '0';
        sacefpd_aresetn : in  std_logic := '0';
        sacefpd_awvalid : in  std_logic := '0';
        sacefpd_awready : out std_logic;
        sacefpd_awaddr  : in  std_logic_vector(S_AXI_ACE_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        sacefpd_awprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        sacefpd_awqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        sacefpd_awcache : in  std_logic_vector(3 downto 0) := (others => '0');
        sacefpd_awburst : in  std_logic_vector(1 downto 0) := (others => '0');
        sacefpd_awsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        sacefpd_awlen   : in  std_logic_vector(S_AXI_ACE_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        sacefpd_awid    : in  std_logic_vector(S_AXI_ACE_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --sacefpd_awlock  : in  std_logic_vector(S_AXI_ACE_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        sacefpd_awlock  : in  std_logic := '0';
        sacefpd_wid     : in  std_logic_vector(S_AXI_ACE_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');  -- Unconnected and unused
        sacefpd_wvalid  : in  std_logic := '0';
        sacefpd_wready  : out std_logic;
        sacefpd_wdata   : in  std_logic_vector(S_AXI_ACE_FPD_DATA_WIDTH_G-1 downto 0) := (others => '0');
        sacefpd_wstrb   : in  std_logic_vector(S_AXI_ACE_FPD_DATA_WIDTH_G/8-1 downto 0) := (others => '0');
        sacefpd_wlast   : in  std_logic := '0';
        sacefpd_bvalid  : out std_logic;
        sacefpd_bready  : in  std_logic := '0';
        sacefpd_bresp   : out std_logic_vector(1 downto 0);
        sacefpd_bid     : out std_logic_vector(S_AXI_ACE_FPD_ID_WIDTH_G-1 downto 0);
        sacefpd_arvalid : in  std_logic := '0';
        sacefpd_arready : out std_logic;
        sacefpd_araddr  : in  std_logic_vector(S_AXI_ACE_FPD_ADDR_WIDTH_G-1 downto 0) := (others => '0');
        sacefpd_arprot  : in  std_logic_vector(2 downto 0) := (others => '0');
        sacefpd_arqos   : in  std_logic_vector(3 downto 0) := (others => '0');
        sacefpd_arcache : in  std_logic_vector(3 downto 0) := (others => '0');
        sacefpd_arburst : in  std_logic_vector(1 downto 0) := (others => '0');
        sacefpd_arsize  : in  std_logic_vector(2 downto 0) := (others => '0');
        sacefpd_arlen   : in  std_logic_vector(S_AXI_ACE_FPD_AXLEN_WIDTH_G-1 downto 0) := (others => '0');
        sacefpd_arid    : in  std_logic_vector(S_AXI_ACE_FPD_ID_WIDTH_G-1 downto 0) := (others => '0');
        --sacefpd_arlock  : in  std_logic_vector(S_AXI_ACE_FPD_AXLOCK_WIDTH_G-1 downto 0) := (others => '0');
        sacefpd_arlock  : in  std_logic := '0';
        sacefpd_rvalid  : out std_logic;
        sacefpd_rready  : in  std_logic := '0';
        sacefpd_rdata   : out std_logic_vector(S_AXI_ACE_FPD_DATA_WIDTH_G-1 downto 0);
        sacefpd_rresp   : out std_logic_vector(3 downto 0);
        sacefpd_rid     : out std_logic_vector(S_AXI_ACE_FPD_ID_WIDTH_G-1 downto 0);
        sacefpd_rlast   : out std_logic;





        -- IRQs
        pl_ps_irq0        : in  std_logic_vector(7 downto 0) := (others => '0');
        pl_ps_irq1        : in  std_logic_vector(7 downto 0) := (others => '0');
        -- Reset
        rst_o             : out std_logic
        );
end entity zynq_ultra_ps_wrapper;

architecture struct of zynq_ultra_ps_wrapper is
begin
    -- This entity is a SystemC entity
    zynqmp_ps_inst : entity work.zynqmp_ps
        generic map (
            QEMU_PATH_TO_SOCKET_G    => QEMU_PATH_TO_SOCKET_G,
            QEMU_SYNC_QUANTUM_G      => QEMU_SYNC_QUANTUM_G,
            FCLK_CLK0_PERIOD_IN_NS_G => FCLK_CLK0_PERIOD_IN_NS_G,
            FCLK_CLK1_PERIOD_IN_NS_G => FCLK_CLK1_PERIOD_IN_NS_G,
            FCLK_CLK2_PERIOD_IN_NS_G => FCLK_CLK2_PERIOD_IN_NS_G,
            FCLK_CLK3_PERIOD_IN_NS_G => FCLK_CLK3_PERIOD_IN_NS_G,
            M_AXI_HPM0_FPD_ENABLE_G       => M_AXI_HPM0_FPD_ENABLE_G,
            M_AXI_HPM1_FPD_ENABLE_G       => M_AXI_HPM1_FPD_ENABLE_G,
            M_AXI_HPM0_LPD_ENABLE_G       => M_AXI_HPM0_LPD_ENABLE_G,
            S_AXI_HPC0_FPD_ENABLE_G       => S_AXI_HPC0_FPD_ENABLE_G,
            S_AXI_HPC1_FPD_ENABLE_G       => S_AXI_HPC1_FPD_ENABLE_G,
            S_AXI_HP0_FPD_ENABLE_G       => S_AXI_HP0_FPD_ENABLE_G,
            S_AXI_HP1_FPD_ENABLE_G       => S_AXI_HP1_FPD_ENABLE_G,
            S_AXI_HP2_FPD_ENABLE_G       => S_AXI_HP2_FPD_ENABLE_G,
            S_AXI_HP3_FPD_ENABLE_G       => S_AXI_HP3_FPD_ENABLE_G,
            S_AXI_LPD_ENABLE_G           => S_AXI_LPD_ENABLE_G,
            S_AXI_ACP_FPD_ENABLE_G       => S_AXI_ACP_FPD_ENABLE_G,
            S_AXI_ACE_FPD_ENABLE_G       => S_AXI_ACE_FPD_ENABLE_G)
        port map (
            -- Clocks
            fclk_clk0          => pl_clk0,
            fclk_clk1          => pl_clk1,
            fclk_clk2          => pl_clk2,
            fclk_clk3          => pl_clk3,
            -- M AXI GP0
            maxigp0_aclk     => maxihpm0_fpd_aclk,
            maxigp0_aresetn  => maxigp0_aresetn,
            maxigp0_awvalid  => maxigp0_awvalid,
            maxigp0_awready  => maxigp0_awready,
            maxigp0_awaddr   => maxigp0_awaddr,
            maxigp0_awprot   => maxigp0_awprot,
            maxigp0_awqos    => maxigp0_awqos,
            maxigp0_awcache  => maxigp0_awcache,
            maxigp0_awburst  => maxigp0_awburst,
            maxigp0_awsize   => maxigp0_awsize,
            maxigp0_awlen    => maxigp0_awlen,
            maxigp0_awid     => maxigp0_awid,
            maxigp0_awlock   => maxigp0_awlock,
            --maxigp0_wid      => maxigp0_wid, -- Only in AXI3 on BFM (tlm2axi-bridge)
            maxigp0_wvalid   => maxigp0_wvalid,
            maxigp0_wready   => maxigp0_wready,
            maxigp0_wdata    => maxigp0_wdata,
            maxigp0_wstrb    => maxigp0_wstrb,
            maxigp0_wlast    => maxigp0_wlast,
            maxigp0_bvalid   => maxigp0_bvalid,
            maxigp0_bready   => maxigp0_bready,
            maxigp0_bresp    => maxigp0_bresp,
            maxigp0_bid      => maxigp0_bid,
            maxigp0_arvalid  => maxigp0_arvalid,
            maxigp0_arready  => maxigp0_arready,
            maxigp0_araddr   => maxigp0_araddr,
            maxigp0_arprot   => maxigp0_arprot,
            maxigp0_arqos    => maxigp0_arqos,
            maxigp0_arcache  => maxigp0_arcache,
            maxigp0_arburst  => maxigp0_arburst,
            maxigp0_arsize   => maxigp0_arsize,
            maxigp0_arlen    => maxigp0_arlen,
            maxigp0_arid     => maxigp0_arid,
            maxigp0_arlock   => maxigp0_arlock,
            maxigp0_rvalid   => maxigp0_rvalid,
            maxigp0_rready   => maxigp0_rready,
            maxigp0_rdata    => maxigp0_rdata,
            maxigp0_rresp    => maxigp0_rresp,
            maxigp0_rid      => maxigp0_rid,
            maxigp0_rlast    => maxigp0_rlast,
            -- M AXI GP1
            maxigp1_aclk     => maxihpm1_fpd_aclk,
            maxigp1_aresetn  => maxigp1_aresetn,
            maxigp1_awvalid  => maxigp1_awvalid,
            maxigp1_awready  => maxigp1_awready,
            maxigp1_awaddr   => maxigp1_awaddr,
            maxigp1_awprot   => maxigp1_awprot,
            maxigp1_awqos    => maxigp1_awqos,
            maxigp1_awcache  => maxigp1_awcache,
            maxigp1_awburst  => maxigp1_awburst,
            maxigp1_awsize   => maxigp1_awsize,
            maxigp1_awlen    => maxigp1_awlen,
            maxigp1_awid     => maxigp1_awid,
            maxigp1_awlock   => maxigp1_awlock,
            --maxigp1_wid      => maxigp1_wid, -- Only in AXI3 on BFM (tlm2axi-bridge)
            maxigp1_wvalid   => maxigp1_wvalid,
            maxigp1_wready   => maxigp1_wready,
            maxigp1_wdata    => maxigp1_wdata,
            maxigp1_wstrb    => maxigp1_wstrb,
            maxigp1_wlast    => maxigp1_wlast,
            maxigp1_bvalid   => maxigp1_bvalid,
            maxigp1_bready   => maxigp1_bready,
            maxigp1_bresp    => maxigp1_bresp,
            maxigp1_bid      => maxigp1_bid,
            maxigp1_arvalid  => maxigp1_arvalid,
            maxigp1_arready  => maxigp1_arready,
            maxigp1_araddr   => maxigp1_araddr,
            maxigp1_arprot   => maxigp1_arprot,
            maxigp1_arqos    => maxigp1_arqos,
            maxigp1_arcache  => maxigp1_arcache,
            maxigp1_arburst  => maxigp1_arburst,
            maxigp1_arsize   => maxigp1_arsize,
            maxigp1_arlen    => maxigp1_arlen,
            maxigp1_arid     => maxigp1_arid,
            maxigp1_arlock   => maxigp1_arlock,
            maxigp1_rvalid   => maxigp1_rvalid,
            maxigp1_rready   => maxigp1_rready,
            maxigp1_rdata    => maxigp1_rdata,
            maxigp1_rresp    => maxigp1_rresp,
            maxigp1_rid      => maxigp1_rid,
            maxigp1_rlast    => maxigp1_rlast,
            -- M AXI GP2
            maxigp2_aclk     => maxihpm1_fpd_aclk,
            maxigp2_aresetn  => maxigp2_aresetn,
            maxigp2_awvalid  => maxigp2_awvalid,
            maxigp2_awready  => maxigp2_awready,
            maxigp2_awaddr   => maxigp2_awaddr,
            maxigp2_awprot   => maxigp2_awprot,
            maxigp2_awqos    => maxigp2_awqos,
            maxigp2_awcache  => maxigp2_awcache,
            maxigp2_awburst  => maxigp2_awburst,
            maxigp2_awsize   => maxigp2_awsize,
            maxigp2_awlen    => maxigp2_awlen,
            maxigp2_awid     => maxigp2_awid,
            maxigp2_awlock   => maxigp2_awlock,
            --maxigp2_wid      => maxigp2_wid, -- Only in AXI3 on BFM (tlm2axi-bridge)
            maxigp2_wvalid   => maxigp2_wvalid,
            maxigp2_wready   => maxigp2_wready,
            maxigp2_wdata    => maxigp2_wdata,
            maxigp2_wstrb    => maxigp2_wstrb,
            maxigp2_wlast    => maxigp2_wlast,
            maxigp2_bvalid   => maxigp2_bvalid,
            maxigp2_bready   => maxigp2_bready,
            maxigp2_bresp    => maxigp2_bresp,
            maxigp2_bid      => maxigp2_bid,
            maxigp2_arvalid  => maxigp2_arvalid,
            maxigp2_arready  => maxigp2_arready,
            maxigp2_araddr   => maxigp2_araddr,
            maxigp2_arprot   => maxigp2_arprot,
            maxigp2_arqos    => maxigp2_arqos,
            maxigp2_arcache  => maxigp2_arcache,
            maxigp2_arburst  => maxigp2_arburst,
            maxigp2_arsize   => maxigp2_arsize,
            maxigp2_arlen    => maxigp2_arlen,
            maxigp2_arid     => maxigp2_arid,
            maxigp2_arlock   => maxigp2_arlock,
            maxigp2_rvalid   => maxigp2_rvalid,
            maxigp2_rready   => maxigp2_rready,
            maxigp2_rdata    => maxigp2_rdata,
            maxigp2_rresp    => maxigp2_rresp,
            maxigp2_rid      => maxigp2_rid,
            maxigp2_rlast    => maxigp2_rlast,
            -- S AXI GP0
            saxigp0_aclk     => saxihpc0_fpd_aclk,
            saxigp0_aresetn  => saxigp0_aresetn,
            saxigp0_awvalid  => saxigp0_awvalid,
            saxigp0_awready  => saxigp0_awready,
            saxigp0_awaddr   => saxigp0_awaddr,
            saxigp0_awprot   => saxigp0_awprot,
            saxigp0_awregion => (others => '0'),
            saxigp0_awqos    => saxigp0_awqos,
            saxigp0_awcache  => saxigp0_awcache,
            saxigp0_awburst  => saxigp0_awburst,
            saxigp0_awsize   => saxigp0_awsize,
            saxigp0_awlen    => saxigp0_awlen,
            saxigp0_awid     => saxigp0_awid,
            saxigp0_awlock   => saxigp0_awlock,
            -- saxigp0_wid => saxigp0_wid, -- Only in AXI3 on BFM (axi2tlm-bridge)
            saxigp0_wvalid   => saxigp0_wvalid,
            saxigp0_wready   => saxigp0_wready,
            saxigp0_wdata    => saxigp0_wdata,
            saxigp0_wstrb    => saxigp0_wstrb,
            saxigp0_wlast    => saxigp0_wlast,
            saxigp0_bvalid   => saxigp0_bvalid,
            saxigp0_bready   => saxigp0_bready,
            saxigp0_bresp    => saxigp0_bresp,
            saxigp0_bid      => saxigp0_bid,
            saxigp0_arvalid  => saxigp0_arvalid,
            saxigp0_arready  => saxigp0_arready,
            saxigp0_araddr   => saxigp0_araddr,
            saxigp0_arprot   => saxigp0_arprot,
            saxigp0_arregion => (others => '0'),
            saxigp0_arqos    => saxigp0_arqos,
            saxigp0_arcache  => saxigp0_arcache,
            saxigp0_arburst  => saxigp0_arburst,
            saxigp0_arsize   => saxigp0_arsize,
            saxigp0_arlen    => saxigp0_arlen,
            saxigp0_arid     => saxigp0_arid,
            saxigp0_arlock   => saxigp0_arlock,
            saxigp0_rvalid   => saxigp0_rvalid,
            saxigp0_rready   => saxigp0_rready,
            saxigp0_rdata    => saxigp0_rdata,
            saxigp0_rresp    => saxigp0_rresp,
            saxigp0_rid      => saxigp0_rid,
            saxigp0_rlast    => saxigp0_rlast,
            -- S AXI GP1
            saxigp1_aclk     => saxihpc1_fpd_aclk,
            saxigp1_aresetn  => saxigp1_aresetn,
            saxigp1_awvalid  => saxigp1_awvalid,
            saxigp1_awready  => saxigp1_awready,
            saxigp1_awaddr   => saxigp1_awaddr,
            saxigp1_awprot   => saxigp1_awprot,
            saxigp1_awregion => (others => '0'),
            saxigp1_awqos    => saxigp1_awqos,
            saxigp1_awcache  => saxigp1_awcache,
            saxigp1_awburst  => saxigp1_awburst,
            saxigp1_awsize   => saxigp1_awsize,
            saxigp1_awlen    => saxigp1_awlen,
            saxigp1_awid     => saxigp1_awid,
            saxigp1_awlock   => saxigp1_awlock,
            -- saxigp1_wid => saxigp1_wid, -- Only in AXI3 on BFM (axi2tlm-bridge)
            saxigp1_wvalid   => saxigp1_wvalid,
            saxigp1_wready   => saxigp1_wready,
            saxigp1_wdata    => saxigp1_wdata,
            saxigp1_wstrb    => saxigp1_wstrb,
            saxigp1_wlast    => saxigp1_wlast,
            saxigp1_bvalid   => saxigp1_bvalid,
            saxigp1_bready   => saxigp1_bready,
            saxigp1_bresp    => saxigp1_bresp,
            saxigp1_bid      => saxigp1_bid,
            saxigp1_arvalid  => saxigp1_arvalid,
            saxigp1_arready  => saxigp1_arready,
            saxigp1_araddr   => saxigp1_araddr,
            saxigp1_arprot   => saxigp1_arprot,
            saxigp1_arregion => (others => '0'),
            saxigp1_arqos    => saxigp1_arqos,
            saxigp1_arcache  => saxigp1_arcache,
            saxigp1_arburst  => saxigp1_arburst,
            saxigp1_arsize   => saxigp1_arsize,
            saxigp1_arlen    => saxigp1_arlen,
            saxigp1_arid     => saxigp1_arid,
            saxigp1_arlock   => saxigp1_arlock,
            saxigp1_rvalid   => saxigp1_rvalid,
            saxigp1_rready   => saxigp1_rready,
            saxigp1_rdata    => saxigp1_rdata,
            saxigp1_rresp    => saxigp1_rresp,
            saxigp1_rid      => saxigp1_rid,
            saxigp1_rlast    => saxigp1_rlast,
            -- S AXI HP0
            saxigp2_aclk     => saxihp0_fpd_aclk,
            saxigp2_aresetn  => saxigp2_aresetn,
            saxigp2_awvalid  => saxigp2_awvalid,
            saxigp2_awready  => saxigp2_awready,
            saxigp2_awaddr   => saxigp2_awaddr,
            saxigp2_awprot   => saxigp2_awprot,
            saxigp2_awregion => (others => '0'),
            saxigp2_awqos    => saxigp2_awqos,
            saxigp2_awcache  => saxigp2_awcache,
            saxigp2_awburst  => saxigp2_awburst,
            saxigp2_awsize   => saxigp2_awsize,
            saxigp2_awlen    => saxigp2_awlen,
            saxigp2_awid     => saxigp2_awid,
            saxigp2_awlock   => saxigp2_awlock,
            -- saxigp2_wid => saxigp2_wid, -- Only in AXI3 on BFM (axi2tlm-bridge)
            saxigp2_wvalid   => saxigp2_wvalid,
            saxigp2_wready   => saxigp2_wready,
            saxigp2_wdata    => saxigp2_wdata,
            saxigp2_wstrb    => saxigp2_wstrb,
            saxigp2_wlast    => saxigp2_wlast,
            saxigp2_bvalid   => saxigp2_bvalid,
            saxigp2_bready   => saxigp2_bready,
            saxigp2_bresp    => saxigp2_bresp,
            saxigp2_bid      => saxigp2_bid,
            saxigp2_arvalid  => saxigp2_arvalid,
            saxigp2_arready  => saxigp2_arready,
            saxigp2_araddr   => saxigp2_araddr,
            saxigp2_arprot   => saxigp2_arprot,
            saxigp2_arregion => (others => '0'),
            saxigp2_arqos    => saxigp2_arqos,
            saxigp2_arcache  => saxigp2_arcache,
            saxigp2_arburst  => saxigp2_arburst,
            saxigp2_arsize   => saxigp2_arsize,
            saxigp2_arlen    => saxigp2_arlen,
            saxigp2_arid     => saxigp2_arid,
            saxigp2_arlock   => saxigp2_arlock,
            saxigp2_rvalid   => saxigp2_rvalid,
            saxigp2_rready   => saxigp2_rready,
            saxigp2_rdata    => saxigp2_rdata,
            saxigp2_rresp    => saxigp2_rresp,
            saxigp2_rid      => saxigp2_rid,
            saxigp2_rlast    => saxigp2_rlast,
            -- S AXI HP1
            saxigp3_aclk     => saxihp1_fpd_aclk,
            saxigp3_aresetn  => saxigp3_aresetn,
            saxigp3_awvalid  => saxigp3_awvalid,
            saxigp3_awready  => saxigp3_awready,
            saxigp3_awaddr   => saxigp3_awaddr,
            saxigp3_awprot   => saxigp3_awprot,
            saxigp3_awregion => (others => '0'),
            saxigp3_awqos    => saxigp3_awqos,
            saxigp3_awcache  => saxigp3_awcache,
            saxigp3_awburst  => saxigp3_awburst,
            saxigp3_awsize   => saxigp3_awsize,
            saxigp3_awlen    => saxigp3_awlen,
            saxigp3_awid     => saxigp3_awid,
            saxigp3_awlock   => saxigp3_awlock,
            -- saxigp3_wid => saxigp3_wid, -- Only in AXI3 on BFM (axi2tlm-bridge)
            saxigp3_wvalid   => saxigp3_wvalid,
            saxigp3_wready   => saxigp3_wready,
            saxigp3_wdata    => saxigp3_wdata,
            saxigp3_wstrb    => saxigp3_wstrb,
            saxigp3_wlast    => saxigp3_wlast,
            saxigp3_bvalid   => saxigp3_bvalid,
            saxigp3_bready   => saxigp3_bready,
            saxigp3_bresp    => saxigp3_bresp,
            saxigp3_bid      => saxigp3_bid,
            saxigp3_arvalid  => saxigp3_arvalid,
            saxigp3_arready  => saxigp3_arready,
            saxigp3_araddr   => saxigp3_araddr,
            saxigp3_arprot   => saxigp3_arprot,
            saxigp3_arregion => (others => '0'),
            saxigp3_arqos    => saxigp3_arqos,
            saxigp3_arcache  => saxigp3_arcache,
            saxigp3_arburst  => saxigp3_arburst,
            saxigp3_arsize   => saxigp3_arsize,
            saxigp3_arlen    => saxigp3_arlen,
            saxigp3_arid     => saxigp3_arid,
            saxigp3_arlock   => saxigp3_arlock,
            saxigp3_rvalid   => saxigp3_rvalid,
            saxigp3_rready   => saxigp3_rready,
            saxigp3_rdata    => saxigp3_rdata,
            saxigp3_rresp    => saxigp3_rresp,
            saxigp3_rid      => saxigp3_rid,
            saxigp3_rlast    => saxigp3_rlast,
            -- S AXI HP2
            saxigp4_aclk     => saxihp2_fpd_aclk,
            saxigp4_aresetn  => saxigp4_aresetn,
            saxigp4_awvalid  => saxigp4_awvalid,
            saxigp4_awready  => saxigp4_awready,
            saxigp4_awaddr   => saxigp4_awaddr,
            saxigp4_awprot   => saxigp4_awprot,
            saxigp4_awregion => (others => '0'),
            saxigp4_awqos    => saxigp4_awqos,
            saxigp4_awcache  => saxigp4_awcache,
            saxigp4_awburst  => saxigp4_awburst,
            saxigp4_awsize   => saxigp4_awsize,
            saxigp4_awlen    => saxigp4_awlen,
            saxigp4_awid     => saxigp4_awid,
            saxigp4_awlock   => saxigp4_awlock,
            -- saxigp4_wid => saxigp4_wid, -- Only in AXI3 on BFM (axi2tlm-bridge)
            saxigp4_wvalid   => saxigp4_wvalid,
            saxigp4_wready   => saxigp4_wready,
            saxigp4_wdata    => saxigp4_wdata,
            saxigp4_wstrb    => saxigp4_wstrb,
            saxigp4_wlast    => saxigp4_wlast,
            saxigp4_bvalid   => saxigp4_bvalid,
            saxigp4_bready   => saxigp4_bready,
            saxigp4_bresp    => saxigp4_bresp,
            saxigp4_bid      => saxigp4_bid,
            saxigp4_arvalid  => saxigp4_arvalid,
            saxigp4_arready  => saxigp4_arready,
            saxigp4_araddr   => saxigp4_araddr,
            saxigp4_arprot   => saxigp4_arprot,
            saxigp4_arregion => (others => '0'),
            saxigp4_arqos    => saxigp4_arqos,
            saxigp4_arcache  => saxigp4_arcache,
            saxigp4_arburst  => saxigp4_arburst,
            saxigp4_arsize   => saxigp4_arsize,
            saxigp4_arlen    => saxigp4_arlen,
            saxigp4_arid     => saxigp4_arid,
            saxigp4_arlock   => saxigp4_arlock,
            saxigp4_rvalid   => saxigp4_rvalid,
            saxigp4_rready   => saxigp4_rready,
            saxigp4_rdata    => saxigp4_rdata,
            saxigp4_rresp    => saxigp4_rresp,
            saxigp4_rid      => saxigp4_rid,
            saxigp4_rlast    => saxigp4_rlast,
            -- S AXI HP3
            saxigp5_aclk     => saxihp3_fpd_aclk,
            saxigp5_aresetn  => saxigp5_aresetn,
            saxigp5_awvalid  => saxigp5_awvalid,
            saxigp5_awready  => saxigp5_awready,
            saxigp5_awaddr   => saxigp5_awaddr,
            saxigp5_awprot   => saxigp5_awprot,
            saxigp5_awregion => (others => '0'),
            saxigp5_awqos    => saxigp5_awqos,
            saxigp5_awcache  => saxigp5_awcache,
            saxigp5_awburst  => saxigp5_awburst,
            saxigp5_awsize   => saxigp5_awsize,
            saxigp5_awlen    => saxigp5_awlen,
            saxigp5_awid     => saxigp5_awid,
            saxigp5_awlock   => saxigp5_awlock,
            -- saxigp5_wid => saxigp5_wid, -- Only in AXI3 on BFM (axi2tlm-bridge)
            saxigp5_wvalid   => saxigp5_wvalid,
            saxigp5_wready   => saxigp5_wready,
            saxigp5_wdata    => saxigp5_wdata,
            saxigp5_wstrb    => saxigp5_wstrb,
            saxigp5_wlast    => saxigp5_wlast,
            saxigp5_bvalid   => saxigp5_bvalid,
            saxigp5_bready   => saxigp5_bready,
            saxigp5_bresp    => saxigp5_bresp,
            saxigp5_bid      => saxigp5_bid,
            saxigp5_arvalid  => saxigp5_arvalid,
            saxigp5_arready  => saxigp5_arready,
            saxigp5_araddr   => saxigp5_araddr,
            saxigp5_arprot   => saxigp5_arprot,
            saxigp5_arregion => (others => '0'),
            saxigp5_arqos    => saxigp5_arqos,
            saxigp5_arcache  => saxigp5_arcache,
            saxigp5_arburst  => saxigp5_arburst,
            saxigp5_arsize   => saxigp5_arsize,
            saxigp5_arlen    => saxigp5_arlen,
            saxigp5_arid     => saxigp5_arid,
            saxigp5_arlock   => saxigp5_arlock,
            saxigp5_rvalid   => saxigp5_rvalid,
            saxigp5_rready   => saxigp5_rready,
            saxigp5_rdata    => saxigp5_rdata,
            saxigp5_rresp    => saxigp5_rresp,
            saxigp5_rid      => saxigp5_rid,
            saxigp5_rlast    => saxigp5_rlast,


            -- S AXI LPD
            saxigp6_aclk     => saxi_lpd_aclk,
            saxigp6_aresetn  => saxigp6_aresetn,
            saxigp6_awvalid  => saxigp6_awvalid,
            saxigp6_awready  => saxigp6_awready,
            saxigp6_awaddr   => saxigp6_awaddr,
            saxigp6_awprot   => saxigp6_awprot,
            saxigp6_awregion => (others => '0'),
            saxigp6_awqos    => saxigp6_awqos,
            saxigp6_awcache  => saxigp6_awcache,
            saxigp6_awburst  => saxigp6_awburst,
            saxigp6_awsize   => saxigp6_awsize,
            saxigp6_awlen    => saxigp6_awlen,
            saxigp6_awid     => saxigp6_awid,
            saxigp6_awlock   => saxigp6_awlock,
            -- saxigp6_wid => saxigp6_wid, -- Only in AXI3 on BFM (axi2tlm-bridge)
            saxigp6_wvalid   => saxigp6_wvalid,
            saxigp6_wready   => saxigp6_wready,
            saxigp6_wdata    => saxigp6_wdata,
            saxigp6_wstrb    => saxigp6_wstrb,
            saxigp6_wlast    => saxigp6_wlast,
            saxigp6_bvalid   => saxigp6_bvalid,
            saxigp6_bready   => saxigp6_bready,
            saxigp6_bresp    => saxigp6_bresp,
            saxigp6_bid      => saxigp6_bid,
            saxigp6_arvalid  => saxigp6_arvalid,
            saxigp6_arready  => saxigp6_arready,
            saxigp6_araddr   => saxigp6_araddr,
            saxigp6_arprot   => saxigp6_arprot,
            saxigp6_arregion => (others => '0'),
            saxigp6_arqos    => saxigp6_arqos,
            saxigp6_arcache  => saxigp6_arcache,
            saxigp6_arburst  => saxigp6_arburst,
            saxigp6_arsize   => saxigp6_arsize,
            saxigp6_arlen    => saxigp6_arlen,
            saxigp6_arid     => saxigp6_arid,
            saxigp6_arlock   => saxigp6_arlock,
            saxigp6_rvalid   => saxigp6_rvalid,
            saxigp6_rready   => saxigp6_rready,
            saxigp6_rdata    => saxigp6_rdata,
            saxigp6_rresp    => saxigp6_rresp,
            saxigp6_rid      => saxigp6_rid,
            saxigp6_rlast    => saxigp6_rlast,

            -- S AXI ACP FPD
            saxiacp_aclk     => saxiacp_fpd_aclk,
            saxiacp_aresetn  => saxiacp_aresetn,
            saxiacp_awvalid  => saxiacp_awvalid,
            saxiacp_awready  => saxiacp_awready,
            saxiacp_awaddr   => saxiacp_awaddr,
            saxiacp_awprot   => saxiacp_awprot,
            saxiacp_awregion => (others => '0'),
            saxiacp_awqos    => saxiacp_awqos,
            saxiacp_awcache  => saxiacp_awcache,
            saxiacp_awburst  => saxiacp_awburst,
            saxiacp_awsize   => saxiacp_awsize,
            saxiacp_awlen    => saxiacp_awlen,
            saxiacp_awid     => saxiacp_awid,
            saxiacp_awlock   => saxiacp_awlock,
            -- saxiacp_wid => saxiacp_wid, -- Only in AXI3 on BFM (axi2tlm-bridge)
            saxiacp_wvalid   => saxiacp_wvalid,
            saxiacp_wready   => saxiacp_wready,
            saxiacp_wdata    => saxiacp_wdata,
            saxiacp_wstrb    => saxiacp_wstrb,
            saxiacp_wlast    => saxiacp_wlast,
            saxiacp_bvalid   => saxiacp_bvalid,
            saxiacp_bready   => saxiacp_bready,
            saxiacp_bresp    => saxiacp_bresp,
            saxiacp_bid      => saxiacp_bid,
            saxiacp_arvalid  => saxiacp_arvalid,
            saxiacp_arready  => saxiacp_arready,
            saxiacp_araddr   => saxiacp_araddr,
            saxiacp_arprot   => saxiacp_arprot,
            saxiacp_arregion => (others => '0'),
            saxiacp_arqos    => saxiacp_arqos,
            saxiacp_arcache  => saxiacp_arcache,
            saxiacp_arburst  => saxiacp_arburst,
            saxiacp_arsize   => saxiacp_arsize,
            saxiacp_arlen    => saxiacp_arlen,
            saxiacp_arid     => saxiacp_arid,
            saxiacp_arlock   => saxiacp_arlock,
            saxiacp_rvalid   => saxiacp_rvalid,
            saxiacp_rready   => saxiacp_rready,
            saxiacp_rdata    => saxiacp_rdata,
            saxiacp_rresp    => saxiacp_rresp,
            saxiacp_rid      => saxiacp_rid,
            saxiacp_rlast    => saxiacp_rlast,


            -- S AXI ACE FPD
            sacefpd_aclk     => sacefpd_aclk,
            sacefpd_aresetn  => sacefpd_aresetn,
            sacefpd_awvalid  => sacefpd_awvalid,
            sacefpd_awready  => sacefpd_awready,
            sacefpd_awaddr   => sacefpd_awaddr,
            sacefpd_awprot   => sacefpd_awprot,
            sacefpd_awregion => (others => '0'),
            sacefpd_awqos    => sacefpd_awqos,
            sacefpd_awcache  => sacefpd_awcache,
            sacefpd_awburst  => sacefpd_awburst,
            sacefpd_awsize   => sacefpd_awsize,
            sacefpd_awlen    => sacefpd_awlen,
            sacefpd_awid     => sacefpd_awid,
            sacefpd_awlock   => sacefpd_awlock,
            -- sacefpd_wid => sacefpd_wid, -- Only in AXI3 on BFM (axi2tlm-bridge)
            sacefpd_wvalid   => sacefpd_wvalid,
            sacefpd_wready   => sacefpd_wready,
            sacefpd_wdata    => sacefpd_wdata,
            sacefpd_wstrb    => sacefpd_wstrb,
            sacefpd_wlast    => sacefpd_wlast,
            sacefpd_bvalid   => sacefpd_bvalid,
            sacefpd_bready   => sacefpd_bready,
            sacefpd_bresp    => sacefpd_bresp,
            sacefpd_bid      => sacefpd_bid,
            sacefpd_arvalid  => sacefpd_arvalid,
            sacefpd_arready  => sacefpd_arready,
            sacefpd_araddr   => sacefpd_araddr,
            sacefpd_arprot   => sacefpd_arprot,
            sacefpd_arregion => (others => '0'),
            sacefpd_arqos    => sacefpd_arqos,
            sacefpd_arcache  => sacefpd_arcache,
            sacefpd_arburst  => sacefpd_arburst,
            sacefpd_arsize   => sacefpd_arsize,
            sacefpd_arlen    => sacefpd_arlen,
            sacefpd_arid     => sacefpd_arid,
            sacefpd_arlock   => sacefpd_arlock,
            sacefpd_rvalid   => sacefpd_rvalid,
            sacefpd_rready   => sacefpd_rready,
            sacefpd_rdata    => sacefpd_rdata,
            sacefpd_rresp    => sacefpd_rresp,
            sacefpd_rid      => sacefpd_rid,
            sacefpd_rlast    => sacefpd_rlast,

            -- IRQs
            pl_ps_irq0       => pl_ps_irq0,
            pl_ps_irq1       => pl_ps_irq1,
            -- Resets
            rst_o              => rst_o);


    assert pl_ps_irq0'length <= 8 report "pl_ps_irq0 port cannot have more than 8 lines" severity warning;
    assert pl_ps_irq1'length <= 8 report "pl_ps_irq0 port cannot have more than 8 lines" severity warning;
    assert QEMU_PATH_TO_SOCKET_G'length <= 108 report "QEMU_PATH_TO_SOCKET_G path is to long, max length is 107 letters" severity warning;

    maxigp0_wid <= (others => '0');
    maxigp1_wid <= (others => '0');
    maxigp2_wid <= (others => '0');

end architecture struct;
