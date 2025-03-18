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
---------------------------------------------------------------------------------
-- Adjusted for Aldec Riviera-PRO - Wojtek Lewandowski 2021 Aldec
-- Adjusted for ZynqMP - Zbyszek Tuchewicz 2022 Aldec
---------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity design_1_zynq_ultra_ps_e_0_0 is
    generic(
        QEMU_PATH_TO_SOCKET_G    : string  := "unix:/path";
        QEMU_SYNC_QUANTUM_G      : integer := 10000;
        --
        FCLK_CLK0_PERIOD_IN_NS_G : integer := 10000;
        FCLK_CLK1_PERIOD_IN_NS_G : integer := -1;
        FCLK_CLK2_PERIOD_IN_NS_G : integer := -1;
        FCLK_CLK3_PERIOD_IN_NS_G : integer := -1;
        --
        M_AXI_HPM0_FPD_ENABLE_G  : integer := 1;
        M_AXI_HPM1_FPD_ENABLE_G  : integer := 0;
        M_AXI_HPM0_LPD_ENABLE_G  : integer := 0;
        --
        S_AXI_HPC0_FPD_ENABLE_G      :  integer := 0;
        S_AXI_HPC1_FPD_ENABLE_G      :  integer := 0;

        S_AXI_HP0_FPD_ENABLE_G       :  integer := 0;
        S_AXI_HP1_FPD_ENABLE_G       :  integer := 0;
        S_AXI_HP2_FPD_ENABLE_G       :  integer := 0;
        S_AXI_HP3_FPD_ENABLE_G       :  integer := 0;
        S_AXI_LPD_ENABLE_G           :  integer := 0;
        S_AXI_ACP_FPD_ENABLE_G           :  integer := 0;
        S_AXI_ACE_FPD_ENABLE_G           :  integer := 0;
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
        S_AXI_HPC0_FPD_ID_WIDTH_G     : integer := 6;
        S_AXI_HPC0_FPD_AXLEN_WIDTH_G  : integer := 8;
        S_AXI_HPC0_FPD_AXLOCK_WIDTH_G : integer := 1;
        --
        S_AXI_HPC1_FPD_ADDR_WIDTH_G   : integer := 49;
        S_AXI_HPC1_FPD_DATA_WIDTH_G   : integer := 128;
        S_AXI_HPC1_FPD_ID_WIDTH_G     : integer := 6;
        S_AXI_HPC1_FPD_AXLEN_WIDTH_G  : integer := 8;
        S_AXI_HPC1_FPD_AXLOCK_WIDTH_G : integer := 1;
        --
        S_AXI_HP0_FPD_ADDR_WIDTH_G   : integer := 49;
        S_AXI_HP0_FPD_DATA_WIDTH_G   : integer := 128;
        S_AXI_HP0_FPD_ID_WIDTH_G     : integer := 6;
        S_AXI_HP0_FPD_AXLEN_WIDTH_G  : integer := 8;
        S_AXI_HP0_FPD_AXLOCK_WIDTH_G : integer := 1;
        --
        S_AXI_HP1_FPD_ADDR_WIDTH_G   : integer := 49;
        S_AXI_HP1_FPD_DATA_WIDTH_G   : integer := 128;
        S_AXI_HP1_FPD_ID_WIDTH_G     : integer := 6;
        S_AXI_HP1_FPD_AXLEN_WIDTH_G  : integer := 8;
        S_AXI_HP1_FPD_AXLOCK_WIDTH_G : integer := 1;
        --
        S_AXI_HP2_FPD_ADDR_WIDTH_G   : integer := 49;
        S_AXI_HP2_FPD_DATA_WIDTH_G   : integer := 128;
        S_AXI_HP2_FPD_ID_WIDTH_G     : integer := 6;
        S_AXI_HP2_FPD_AXLEN_WIDTH_G  : integer := 8;
        S_AXI_HP2_FPD_AXLOCK_WIDTH_G : integer := 1;
        --
        S_AXI_HP3_FPD_ADDR_WIDTH_G   : integer := 49;
        S_AXI_HP3_FPD_DATA_WIDTH_G   : integer := 128;
        S_AXI_HP3_FPD_ID_WIDTH_G     : integer := 6;
        S_AXI_HP3_FPD_AXLEN_WIDTH_G  : integer := 8;
        S_AXI_HP3_FPD_AXLOCK_WIDTH_G : integer := 1;

        S_AXI_LPD_ADDR_WIDTH_G       : integer := 49;
        S_AXI_LPD_DATA_WIDTH_G       : integer := 128;
        S_AXI_LPD_ID_WIDTH_G         : integer := 6;
        S_AXI_LPD_AXLEN_WIDTH_G      : integer := 8;
        S_AXI_LPD_AXLOCK_WIDTH_G     : integer := 1;

        S_AXI_ACP_FPD_ADDR_WIDTH_G       : integer := 40;
        S_AXI_ACP_FPD_DATA_WIDTH_G       : integer := 128;
        S_AXI_ACP_FPD_ID_WIDTH_G         : integer := 5;
        S_AXI_ACP_FPD_AXLEN_WIDTH_G      : integer := 8;
        S_AXI_ACP_FPD_AXLOCK_WIDTH_G     : integer := 1;

        S_AXI_ACE_FPD_ADDR_WIDTH_G       : integer := 44;
        S_AXI_ACE_FPD_DATA_WIDTH_G       : integer := 128;
        S_AXI_ACE_FPD_ID_WIDTH_G         : integer := 6;
        S_AXI_ACE_FPD_AXLEN_WIDTH_G      : integer := 8;
        S_AXI_ACE_FPD_AXLOCK_WIDTH_G     : integer := 1
        );
  Port ( 
    maxihpm0_fpd_aclk : in STD_LOGIC;
    maxigp0_awid : out STD_LOGIC_VECTOR ( 15 downto 0 );
    maxigp0_awaddr : out STD_LOGIC_VECTOR ( 39 downto 0 );
    maxigp0_awlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    maxigp0_awsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    maxigp0_awburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    maxigp0_awlock : out STD_LOGIC;
    maxigp0_awcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    maxigp0_awprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    maxigp0_awvalid : out STD_LOGIC;
    maxigp0_awuser : out STD_LOGIC_VECTOR ( 15 downto 0 );
    maxigp0_awready : in STD_LOGIC;
    maxigp0_wdata : out STD_LOGIC_VECTOR ( 127 downto 0 );
    maxigp0_wstrb : out STD_LOGIC_VECTOR ( 15 downto 0 );
    maxigp0_wlast : out STD_LOGIC;
    maxigp0_wvalid : out STD_LOGIC;
    maxigp0_wready : in STD_LOGIC;
    maxigp0_bid : in STD_LOGIC_VECTOR ( 15 downto 0 );
    maxigp0_bresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    maxigp0_bvalid : in STD_LOGIC;
    maxigp0_bready : out STD_LOGIC;
    maxigp0_arid : out STD_LOGIC_VECTOR ( 15 downto 0 );
    maxigp0_araddr : out STD_LOGIC_VECTOR ( 39 downto 0 );
    maxigp0_arlen : out STD_LOGIC_VECTOR ( 7 downto 0 );
    maxigp0_arsize : out STD_LOGIC_VECTOR ( 2 downto 0 );
    maxigp0_arburst : out STD_LOGIC_VECTOR ( 1 downto 0 );
    maxigp0_arlock : out STD_LOGIC;
    maxigp0_arcache : out STD_LOGIC_VECTOR ( 3 downto 0 );
    maxigp0_arprot : out STD_LOGIC_VECTOR ( 2 downto 0 );
    maxigp0_arvalid : out STD_LOGIC;
    maxigp0_aruser : out STD_LOGIC_VECTOR ( 15 downto 0 );
    maxigp0_arready : in STD_LOGIC;
    maxigp0_rid : in STD_LOGIC_VECTOR ( 15 downto 0 );
    maxigp0_rdata : in STD_LOGIC_VECTOR ( 127 downto 0 );
    maxigp0_rresp : in STD_LOGIC_VECTOR ( 1 downto 0 );
    maxigp0_rlast : in STD_LOGIC;
    maxigp0_rvalid : in STD_LOGIC;
    maxigp0_rready : out STD_LOGIC;
    maxigp0_awqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    maxigp0_arqos : out STD_LOGIC_VECTOR ( 3 downto 0 );
    emio_enet3_mdio_mdc : out STD_LOGIC;
    emio_enet3_mdio_i : in STD_LOGIC;
    emio_enet3_mdio_o : out STD_LOGIC;
    emio_enet3_mdio_t : out STD_LOGIC;
    emio_gpio_i : in STD_LOGIC_VECTOR ( 94 downto 0 );
    emio_gpio_o : out STD_LOGIC_VECTOR ( 94 downto 0 );
    emio_gpio_t : out STD_LOGIC_VECTOR ( 94 downto 0 );
    pl_ps_irq0 : in STD_LOGIC_VECTOR ( 0 to 0 );
    pl_ps_irq1 : in STD_LOGIC_VECTOR ( 0 to 0 );
    pl_resetn0 : out STD_LOGIC;
    pl_clk0 : out STD_LOGIC
  );

end entity design_1_zynq_ultra_ps_e_0_0;

architecture struct of design_1_zynq_ultra_ps_e_0_0 is

    signal rst_s               : std_logic;
    signal maxigp0_aresetn_s : std_logic;
    signal maxigp1_aresetn_s : std_logic;
    signal maxigp2_aresetn_s : std_logic;
    signal saxigp0_aresetn_s : std_logic;
    signal saxigp1_aresetn_s : std_logic;
    signal saxigp2_aresetn_s : std_logic;
    signal saxigp3_aresetn_s : std_logic;
    signal saxigp4_aresetn_s : std_logic;
    signal saxigp5_aresetn_s : std_logic;
    signal saxigp6_aresetn_s : std_logic;
    signal saxiacp_aresetn_s : std_logic;
    signal sacefpd_aresetn_s : std_logic;

begin

    zynq_ultra_ps_wrapper_inst : entity work.zynq_ultra_ps_wrapper
        generic map (
            QEMU_PATH_TO_SOCKET_G    => QEMU_PATH_TO_SOCKET_G,
            QEMU_SYNC_QUANTUM_G      => QEMU_SYNC_QUANTUM_G,
            --
            FCLK_CLK0_PERIOD_IN_NS_G => FCLK_CLK0_PERIOD_IN_NS_G,
            FCLK_CLK1_PERIOD_IN_NS_G => FCLK_CLK1_PERIOD_IN_NS_G,
            FCLK_CLK2_PERIOD_IN_NS_G => FCLK_CLK2_PERIOD_IN_NS_G,
            FCLK_CLK3_PERIOD_IN_NS_G => FCLK_CLK3_PERIOD_IN_NS_G,
            --
            M_AXI_HPM0_FPD_ENABLE_G       => M_AXI_HPM0_FPD_ENABLE_G,
            M_AXI_HPM1_FPD_ENABLE_G       => M_AXI_HPM1_FPD_ENABLE_G,
            M_AXI_HPM0_LPD_ENABLE_G       => M_AXI_HPM0_LPD_ENABLE_G,
            --
            S_AXI_HPC0_FPD_ENABLE_G       => S_AXI_HPC0_FPD_ENABLE_G,
            S_AXI_HPC1_FPD_ENABLE_G       => S_AXI_HPC1_FPD_ENABLE_G,
            --
            S_AXI_HP0_FPD_ENABLE_G       => S_AXI_HP0_FPD_ENABLE_G,
            S_AXI_HP1_FPD_ENABLE_G       => S_AXI_HP1_FPD_ENABLE_G,
            S_AXI_HP2_FPD_ENABLE_G       => S_AXI_HP2_FPD_ENABLE_G,
            S_AXI_HP3_FPD_ENABLE_G       => S_AXI_HP3_FPD_ENABLE_G,
            --
            M_AXI_HPM0_FPD_ADDR_WIDTH_G   => M_AXI_HPM0_FPD_ADDR_WIDTH_G,
            M_AXI_HPM0_FPD_DATA_WIDTH_G   => M_AXI_HPM0_FPD_DATA_WIDTH_G,
            M_AXI_HPM0_FPD_ID_WIDTH_G     => M_AXI_HPM0_FPD_ID_WIDTH_G,
            M_AXI_HPM0_FPD_AXLEN_WIDTH_G  => M_AXI_HPM0_FPD_AXLEN_WIDTH_G,
            M_AXI_HPM0_FPD_AXLOCK_WIDTH_G => M_AXI_HPM0_FPD_AXLOCK_WIDTH_G,
            --
            M_AXI_HPM1_FPD_ADDR_WIDTH_G   => M_AXI_HPM1_FPD_ADDR_WIDTH_G,
            M_AXI_HPM1_FPD_DATA_WIDTH_G   => M_AXI_HPM1_FPD_DATA_WIDTH_G,
            M_AXI_HPM1_FPD_ID_WIDTH_G     => M_AXI_HPM1_FPD_ID_WIDTH_G,
            M_AXI_HPM1_FPD_AXLEN_WIDTH_G  => M_AXI_HPM1_FPD_AXLEN_WIDTH_G,
            M_AXI_HPM1_FPD_AXLOCK_WIDTH_G => M_AXI_HPM1_FPD_AXLOCK_WIDTH_G,
            --
            S_AXI_HPC0_FPD_ADDR_WIDTH_G   => S_AXI_HPC0_FPD_ADDR_WIDTH_G,
            S_AXI_HPC0_FPD_DATA_WIDTH_G   => S_AXI_HPC0_FPD_DATA_WIDTH_G,
            S_AXI_HPC0_FPD_ID_WIDTH_G     => S_AXI_HPC0_FPD_ID_WIDTH_G,
            S_AXI_HPC0_FPD_AXLEN_WIDTH_G  => S_AXI_HPC0_FPD_AXLEN_WIDTH_G,
            S_AXI_HPC0_FPD_AXLOCK_WIDTH_G => S_AXI_HPC0_FPD_AXLOCK_WIDTH_G,
            --
            S_AXI_HPC1_FPD_ADDR_WIDTH_G   => S_AXI_HPC1_FPD_ADDR_WIDTH_G,
            S_AXI_HPC1_FPD_DATA_WIDTH_G   => S_AXI_HPC1_FPD_DATA_WIDTH_G,
            S_AXI_HPC1_FPD_ID_WIDTH_G     => S_AXI_HPC1_FPD_ID_WIDTH_G,
            S_AXI_HPC1_FPD_AXLEN_WIDTH_G  => S_AXI_HPC1_FPD_AXLEN_WIDTH_G,
            S_AXI_HPC1_FPD_AXLOCK_WIDTH_G => S_AXI_HPC1_FPD_AXLOCK_WIDTH_G,
            --
            S_AXI_HP0_FPD_ADDR_WIDTH_G   => S_AXI_HP0_FPD_ADDR_WIDTH_G,
            S_AXI_HP0_FPD_DATA_WIDTH_G   => S_AXI_HP0_FPD_DATA_WIDTH_G,
            S_AXI_HP0_FPD_ID_WIDTH_G     => S_AXI_HP0_FPD_ID_WIDTH_G,
            S_AXI_HP0_FPD_AXLEN_WIDTH_G  => S_AXI_HP0_FPD_AXLEN_WIDTH_G,
            S_AXI_HP0_FPD_AXLOCK_WIDTH_G => S_AXI_HP0_FPD_AXLOCK_WIDTH_G,
            --
            S_AXI_HP1_FPD_ADDR_WIDTH_G   => S_AXI_HP1_FPD_ADDR_WIDTH_G,
            S_AXI_HP1_FPD_DATA_WIDTH_G   => S_AXI_HP1_FPD_DATA_WIDTH_G,
            S_AXI_HP1_FPD_ID_WIDTH_G     => S_AXI_HP1_FPD_ID_WIDTH_G,
            S_AXI_HP1_FPD_AXLEN_WIDTH_G  => S_AXI_HP1_FPD_AXLEN_WIDTH_G,
            S_AXI_HP1_FPD_AXLOCK_WIDTH_G => S_AXI_HP1_FPD_AXLOCK_WIDTH_G,
            --
            S_AXI_HP2_FPD_ADDR_WIDTH_G   => S_AXI_HP2_FPD_ADDR_WIDTH_G,
            S_AXI_HP2_FPD_DATA_WIDTH_G   => S_AXI_HP2_FPD_DATA_WIDTH_G,
            S_AXI_HP2_FPD_ID_WIDTH_G     => S_AXI_HP2_FPD_ID_WIDTH_G,
            S_AXI_HP2_FPD_AXLEN_WIDTH_G  => S_AXI_HP2_FPD_AXLEN_WIDTH_G,
            S_AXI_HP2_FPD_AXLOCK_WIDTH_G => S_AXI_HP2_FPD_AXLOCK_WIDTH_G,
            --
            S_AXI_HP3_FPD_ADDR_WIDTH_G   => S_AXI_HP3_FPD_ADDR_WIDTH_G,
            S_AXI_HP3_FPD_DATA_WIDTH_G   => S_AXI_HP3_FPD_DATA_WIDTH_G,
            S_AXI_HP3_FPD_ID_WIDTH_G     => S_AXI_HP3_FPD_ID_WIDTH_G,
            S_AXI_HP3_FPD_AXLEN_WIDTH_G  => S_AXI_HP3_FPD_AXLEN_WIDTH_G,
            S_AXI_HP3_FPD_AXLOCK_WIDTH_G => S_AXI_HP3_FPD_AXLOCK_WIDTH_G,
            --
            S_AXI_LPD_ADDR_WIDTH_G       => S_AXI_LPD_ADDR_WIDTH_G,
            S_AXI_LPD_DATA_WIDTH_G       => S_AXI_LPD_DATA_WIDTH_G,
            S_AXI_LPD_ID_WIDTH_G         => S_AXI_LPD_ID_WIDTH_G,
            S_AXI_LPD_AXLEN_WIDTH_G      => S_AXI_LPD_AXLEN_WIDTH_G,
            S_AXI_LPD_AXLOCK_WIDTH_G     => S_AXI_LPD_AXLOCK_WIDTH_G,

            S_AXI_ACP_FPD_ADDR_WIDTH_G       => S_AXI_ACP_FPD_ADDR_WIDTH_G,
            S_AXI_ACP_FPD_DATA_WIDTH_G       => S_AXI_ACP_FPD_DATA_WIDTH_G,
            S_AXI_ACP_FPD_ID_WIDTH_G         => S_AXI_ACP_FPD_ID_WIDTH_G,
            S_AXI_ACP_FPD_AXLEN_WIDTH_G      => S_AXI_ACP_FPD_AXLEN_WIDTH_G,
            S_AXI_ACP_FPD_AXLOCK_WIDTH_G     => S_AXI_ACP_FPD_AXLOCK_WIDTH_G,

            S_AXI_ACE_FPD_ADDR_WIDTH_G       => S_AXI_ACE_FPD_ADDR_WIDTH_G,
            S_AXI_ACE_FPD_DATA_WIDTH_G       => S_AXI_ACE_FPD_DATA_WIDTH_G,
            S_AXI_ACE_FPD_ID_WIDTH_G         => S_AXI_ACE_FPD_ID_WIDTH_G,
            S_AXI_ACE_FPD_AXLEN_WIDTH_G      => S_AXI_ACE_FPD_AXLEN_WIDTH_G,
            S_AXI_ACE_FPD_AXLOCK_WIDTH_G     => S_AXI_ACE_FPD_AXLOCK_WIDTH_G)
            
        port map (
            --__CONNECT_PL_CLK0__
            pl_clk0         => pl_clk0,
            --__CONNECT_CLK_MAXIHPM0_FPD__
            maxihpm0_fpd_aclk    => MAXIHPM0_FPD_ACLK,
            --__CONNECT_MAXIGP0__
            maxigp0_aresetn => maxigp0_aresetn_s,
            maxigp0_awvalid => MAXIGP0_AWVALID,
            maxigp0_awready => MAXIGP0_AWREADY,
            maxigp0_awaddr  => MAXIGP0_AWADDR,
            maxigp0_awprot  => MAXIGP0_AWPROT,
            maxigp0_awqos   => MAXIGP0_AWQOS,
            maxigp0_awcache => MAXIGP0_AWCACHE,
            maxigp0_awburst => MAXIGP0_AWBURST,
            maxigp0_awsize  => MAXIGP0_AWSIZE,
            maxigp0_awlen   => MAXIGP0_AWLEN,
            maxigp0_awid    => MAXIGP0_AWID,
            maxigp0_awlock  => MAXIGP0_AWLOCK,
            maxigp0_wvalid  => MAXIGP0_WVALID,
            maxigp0_wready  => MAXIGP0_WREADY,
            maxigp0_wdata   => MAXIGP0_WDATA,
            maxigp0_wstrb   => MAXIGP0_WSTRB,
            maxigp0_wlast   => MAXIGP0_WLAST,
            maxigp0_bvalid  => MAXIGP0_BVALID,
            maxigp0_bready  => MAXIGP0_BREADY,
            maxigp0_bresp   => MAXIGP0_BRESP,
            maxigp0_bid     => MAXIGP0_BID,
            maxigp0_arvalid => MAXIGP0_ARVALID,
            maxigp0_arready => MAXIGP0_ARREADY,
            maxigp0_araddr  => MAXIGP0_ARADDR,
            maxigp0_arprot  => MAXIGP0_ARPROT,
            maxigp0_arqos   => MAXIGP0_ARQOS,
            maxigp0_arcache => MAXIGP0_ARCACHE,
            maxigp0_arburst => MAXIGP0_ARBURST,
            maxigp0_arsize  => MAXIGP0_ARSIZE,
            maxigp0_arlen   => MAXIGP0_ARLEN,
            maxigp0_arid    => MAXIGP0_ARID,
            maxigp0_arlock  => MAXIGP0_ARLOCK,
            maxigp0_rvalid  => MAXIGP0_RVALID,
            maxigp0_rready  => MAXIGP0_RREADY,
            maxigp0_rdata   => MAXIGP0_RDATA,
            maxigp0_rresp   => MAXIGP0_RRESP,
            maxigp0_rid     => MAXIGP0_RID,
            maxigp0_rlast   => MAXIGP0_RLAST,
            --__CONNECT_IRQ_F2P0__
            pl_ps_irq0           => std_logic_vector(resize(unsigned(pl_ps_irq0), 8)),
            
            --__CONNECT_IRQ_F2P1__
            pl_ps_irq1           => std_logic_vector(resize(unsigned(pl_ps_irq1), 8)),
            
            --__
            --
            rst_o             => rst_s);

    pl_resetn0 <= not rst_s;

    --
    maxigp0_aresetn_s <= not rst_s;
    maxigp1_aresetn_s <= not rst_s;
    maxigp2_aresetn_s <= not rst_s;
    --
    saxigp0_aresetn_s <= not rst_s;
    saxigp1_aresetn_s <= not rst_s;
    --
    saxigp0_aresetn_s <= not rst_s;
    saxigp1_aresetn_s <= not rst_s;
    --
    saxigp2_aresetn_s <= not rst_s;
    saxigp3_aresetn_s <= not rst_s;
    saxigp4_aresetn_s <= not rst_s;
    saxigp5_aresetn_s <= not rst_s;
    saxigp6_aresetn_s <= not rst_s;
    --
    saxiacp_aresetn_s <= not rst_s;
    sacefpd_aresetn_s <= not rst_s;
    --

    -- Other inputs and outputs are left unconnected because they are not
    -- needed for this design and will therefore stay in an 'U' or 'X' state in
    -- simulation. They should not connected to anything meaningful in the top
    -- design.

end architecture struct;