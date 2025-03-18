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
---------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity design_1_processing_system7_0_0 is
    generic(
        QEMU_PATH_TO_SOCKET_G    : string  := "unix:/path";
        QEMU_SYNC_QUANTUM_G      : integer := 10000;
        --
        FCLK_CLK0_PERIOD_IN_NS_G : integer := 10000;
        FCLK_CLK1_PERIOD_IN_NS_G : integer := 10000;
        FCLK_CLK2_PERIOD_IN_NS_G : integer := -1;
        FCLK_CLK3_PERIOD_IN_NS_G : integer := -1;
        --
        M_AXI_GP0_ENABLE_G       : integer := 1;
        M_AXI_GP1_ENABLE_G       : integer := 0;
        --
        S_AXI_GP0_ENABLE_G       : integer := 0;
        S_AXI_GP1_ENABLE_G       : integer := 0;
        S_AXI_HP0_ENABLE_G       : integer := 0;
        S_AXI_HP1_ENABLE_G       : integer := 0;
        S_AXI_HP2_ENABLE_G       : integer := 0;
        S_AXI_HP3_ENABLE_G       : integer := 0;
        --
        M_AXI_GP0_ADDR_WIDTH_G   : integer := 32;
        M_AXI_GP0_DATA_WIDTH_G   : integer := 32;
        M_AXI_GP0_ID_WIDTH_G     : integer := 12;
        M_AXI_GP0_AXLEN_WIDTH_G  : integer := 4;
        M_AXI_GP0_AXLOCK_WIDTH_G : integer := 2;
        --
        M_AXI_GP1_ADDR_WIDTH_G   : integer := 32;
        M_AXI_GP1_DATA_WIDTH_G   : integer := 32;
        M_AXI_GP1_ID_WIDTH_G     : integer := 12;
        M_AXI_GP1_AXLEN_WIDTH_G  : integer := 4;
        M_AXI_GP1_AXLOCK_WIDTH_G : integer := 2;
        --
        S_AXI_GP0_ADDR_WIDTH_G   : integer := 32;
        S_AXI_GP0_DATA_WIDTH_G   : integer := 32;
        S_AXI_GP0_ID_WIDTH_G     : integer := 6;
        S_AXI_GP0_AXLEN_WIDTH_G  : integer := 4;
        S_AXI_GP0_AXLOCK_WIDTH_G : integer := 2;
        --
        S_AXI_GP1_ADDR_WIDTH_G   : integer := 32;
        S_AXI_GP1_DATA_WIDTH_G   : integer := 32;
        S_AXI_GP1_ID_WIDTH_G     : integer := 6;
        S_AXI_GP1_AXLEN_WIDTH_G  : integer := 4;
        S_AXI_GP1_AXLOCK_WIDTH_G : integer := 2;
        --
        S_AXI_HP0_ADDR_WIDTH_G   : integer := 32;
        S_AXI_HP0_DATA_WIDTH_G   : integer := 64;
        S_AXI_HP0_ID_WIDTH_G     : integer := 6;
        S_AXI_HP0_AXLEN_WIDTH_G  : integer := 4;
        S_AXI_HP0_AXLOCK_WIDTH_G : integer := 2;
        --
        S_AXI_HP1_ADDR_WIDTH_G   : integer := 32;
        S_AXI_HP1_DATA_WIDTH_G   : integer := 64;
        S_AXI_HP1_ID_WIDTH_G     : integer := 6;
        S_AXI_HP1_AXLEN_WIDTH_G  : integer := 4;
        S_AXI_HP1_AXLOCK_WIDTH_G : integer := 2;
        --
        S_AXI_HP2_ADDR_WIDTH_G   : integer := 32;
        S_AXI_HP2_DATA_WIDTH_G   : integer := 64;
        S_AXI_HP2_ID_WIDTH_G     : integer := 6;
        S_AXI_HP2_AXLEN_WIDTH_G  : integer := 4;
        S_AXI_HP2_AXLOCK_WIDTH_G : integer := 2;
        --
        S_AXI_HP3_ADDR_WIDTH_G   : integer := 32;
        S_AXI_HP3_DATA_WIDTH_G   : integer := 64;
        S_AXI_HP3_ID_WIDTH_G     : integer := 6;
        S_AXI_HP3_AXLEN_WIDTH_G  : integer := 4;
        S_AXI_HP3_AXLOCK_WIDTH_G : integer := 2
        );
  Port ( 
    SDIO0_CDN : in STD_LOGIC;
    SDIO0_WP : in STD_LOGIC;
    USB0_PORT_INDCTL : out STD_LOGIC_VECTOR ( 1 downto 0 );
    USB0_VBUS_PWRSELECT : out STD_LOGIC;
    USB0_VBUS_PWRFAULT : in STD_LOGIC;
    M_AXI_GP0_ARVALID : out STD_LOGIC;
    M_AXI_GP0_AWVALID : out STD_LOGIC;
    M_AXI_GP0_BREADY : out STD_LOGIC;
    M_AXI_GP0_RREADY : out STD_LOGIC;
    M_AXI_GP0_WLAST : out STD_LOGIC;
    M_AXI_GP0_WVALID : out STD_LOGIC;
    M_AXI_GP0_ARID : out STD_LOGIC_VECTOR ( 11 downto 0 );
    M_AXI_GP0_AWID : out STD_LOGIC_VECTOR ( 11 downto 0 );
    M_AXI_GP0_WID : out STD_LOGIC_VECTOR ( 11 downto 0 );
    M_AXI_GP0_ARBURST : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M_AXI_GP0_ARLOCK : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M_AXI_GP0_ARSIZE : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M_AXI_GP0_AWBURST : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M_AXI_GP0_AWLOCK : out STD_LOGIC_VECTOR ( 1 downto 0 );
    M_AXI_GP0_AWSIZE : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M_AXI_GP0_ARPROT : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M_AXI_GP0_AWPROT : out STD_LOGIC_VECTOR ( 2 downto 0 );
    M_AXI_GP0_ARADDR : out STD_LOGIC_VECTOR ( 31 downto 0 );
    M_AXI_GP0_AWADDR : out STD_LOGIC_VECTOR ( 31 downto 0 );
    M_AXI_GP0_WDATA : out STD_LOGIC_VECTOR ( 31 downto 0 );
    M_AXI_GP0_ARCACHE : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M_AXI_GP0_ARLEN : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M_AXI_GP0_ARQOS : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M_AXI_GP0_AWCACHE : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M_AXI_GP0_AWLEN : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M_AXI_GP0_AWQOS : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M_AXI_GP0_WSTRB : out STD_LOGIC_VECTOR ( 3 downto 0 );
    M_AXI_GP0_ACLK : in STD_LOGIC;
    M_AXI_GP0_ARREADY : in STD_LOGIC;
    M_AXI_GP0_AWREADY : in STD_LOGIC;
    M_AXI_GP0_BVALID : in STD_LOGIC;
    M_AXI_GP0_RLAST : in STD_LOGIC;
    M_AXI_GP0_RVALID : in STD_LOGIC;
    M_AXI_GP0_WREADY : in STD_LOGIC;
    M_AXI_GP0_BID : in STD_LOGIC_VECTOR ( 11 downto 0 );
    M_AXI_GP0_RID : in STD_LOGIC_VECTOR ( 11 downto 0 );
    M_AXI_GP0_BRESP : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M_AXI_GP0_RRESP : in STD_LOGIC_VECTOR ( 1 downto 0 );
    M_AXI_GP0_RDATA : in STD_LOGIC_VECTOR ( 31 downto 0 );
    FCLK_CLK0 : out STD_LOGIC;
    FCLK_CLK1 : out STD_LOGIC;
    FCLK_RESET0_N : out STD_LOGIC;
    MIO : inout STD_LOGIC_VECTOR ( 53 downto 0 );
    DDR_CAS_n : inout STD_LOGIC;
    DDR_CKE : inout STD_LOGIC;
    DDR_Clk_n : inout STD_LOGIC;
    DDR_Clk : inout STD_LOGIC;
    DDR_CS_n : inout STD_LOGIC;
    DDR_DRSTB : inout STD_LOGIC;
    DDR_ODT : inout STD_LOGIC;
    DDR_RAS_n : inout STD_LOGIC;
    DDR_WEB : inout STD_LOGIC;
    DDR_BankAddr : inout STD_LOGIC_VECTOR ( 2 downto 0 );
    DDR_Addr : inout STD_LOGIC_VECTOR ( 14 downto 0 );
    DDR_VRN : inout STD_LOGIC;
    DDR_VRP : inout STD_LOGIC;
    DDR_DM : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_DQ : inout STD_LOGIC_VECTOR ( 31 downto 0 );
    DDR_DQS_n : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_DQS : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    PS_SRSTB : inout STD_LOGIC;
    PS_CLK : inout STD_LOGIC;
    PS_PORB : inout STD_LOGIC
  );

end entity design_1_processing_system7_0_0;

architecture struct of design_1_processing_system7_0_0 is

    signal rst_s               : std_logic;
    signal m_axi_gp0_aresetn_s : std_logic;
    signal m_axi_gp1_aresetn_s : std_logic;
    signal s_axi_gp0_aresetn_s : std_logic;
    signal s_axi_gp1_aresetn_s : std_logic;
    signal s_axi_hp0_aresetn_s : std_logic;
    signal s_axi_hp1_aresetn_s : std_logic;
    signal s_axi_hp2_aresetn_s : std_logic;
    signal s_axi_hp3_aresetn_s : std_logic;

begin

    zynq7_ps_wrapper_inst : entity work.zynq7_ps_wrapper
        generic map (
            QEMU_PATH_TO_SOCKET_G    => QEMU_PATH_TO_SOCKET_G,
            QEMU_SYNC_QUANTUM_G      => QEMU_SYNC_QUANTUM_G,
            --
            FCLK_CLK0_PERIOD_IN_NS_G => FCLK_CLK0_PERIOD_IN_NS_G,
            FCLK_CLK1_PERIOD_IN_NS_G => FCLK_CLK1_PERIOD_IN_NS_G,
            FCLK_CLK2_PERIOD_IN_NS_G => FCLK_CLK2_PERIOD_IN_NS_G,
            FCLK_CLK3_PERIOD_IN_NS_G => FCLK_CLK3_PERIOD_IN_NS_G,
            --
            M_AXI_GP0_ENABLE_G       => M_AXI_GP0_ENABLE_G,
            M_AXI_GP1_ENABLE_G       => M_AXI_GP1_ENABLE_G,
            --
            S_AXI_GP0_ENABLE_G       => S_AXI_GP0_ENABLE_G,
            S_AXI_GP1_ENABLE_G       => S_AXI_GP1_ENABLE_G,
            --
            S_AXI_HP0_ENABLE_G       => S_AXI_HP0_ENABLE_G,
            S_AXI_HP1_ENABLE_G       => S_AXI_HP1_ENABLE_G,
            S_AXI_HP2_ENABLE_G       => S_AXI_HP2_ENABLE_G,
            S_AXI_HP3_ENABLE_G       => S_AXI_HP3_ENABLE_G,
            --
            M_AXI_GP0_ADDR_WIDTH_G   => M_AXI_GP0_ADDR_WIDTH_G,
            M_AXI_GP0_DATA_WIDTH_G   => M_AXI_GP0_DATA_WIDTH_G,
            M_AXI_GP0_ID_WIDTH_G     => M_AXI_GP0_ID_WIDTH_G,
            M_AXI_GP0_AXLEN_WIDTH_G  => M_AXI_GP0_AXLEN_WIDTH_G,
            M_AXI_GP0_AXLOCK_WIDTH_G => M_AXI_GP0_AXLOCK_WIDTH_G,
            --
            M_AXI_GP1_ADDR_WIDTH_G   => M_AXI_GP1_ADDR_WIDTH_G,
            M_AXI_GP1_DATA_WIDTH_G   => M_AXI_GP1_DATA_WIDTH_G,
            M_AXI_GP1_ID_WIDTH_G     => M_AXI_GP1_ID_WIDTH_G,
            M_AXI_GP1_AXLEN_WIDTH_G  => M_AXI_GP1_AXLEN_WIDTH_G,
            M_AXI_GP1_AXLOCK_WIDTH_G => M_AXI_GP1_AXLOCK_WIDTH_G,
            --
            S_AXI_GP0_ADDR_WIDTH_G   => S_AXI_GP0_ADDR_WIDTH_G,
            S_AXI_GP0_DATA_WIDTH_G   => S_AXI_GP0_DATA_WIDTH_G,
            S_AXI_GP0_ID_WIDTH_G     => S_AXI_GP0_ID_WIDTH_G,
            S_AXI_GP0_AXLEN_WIDTH_G  => S_AXI_GP0_AXLEN_WIDTH_G,
            S_AXI_GP0_AXLOCK_WIDTH_G => S_AXI_GP0_AXLOCK_WIDTH_G,
            --
            S_AXI_GP1_ADDR_WIDTH_G   => S_AXI_GP1_ADDR_WIDTH_G,
            S_AXI_GP1_DATA_WIDTH_G   => S_AXI_GP1_DATA_WIDTH_G,
            S_AXI_GP1_ID_WIDTH_G     => S_AXI_GP1_ID_WIDTH_G,
            S_AXI_GP1_AXLEN_WIDTH_G  => S_AXI_GP1_AXLEN_WIDTH_G,
            S_AXI_GP1_AXLOCK_WIDTH_G => S_AXI_GP1_AXLOCK_WIDTH_G,
            --
            S_AXI_HP0_ADDR_WIDTH_G   => S_AXI_HP0_ADDR_WIDTH_G,
            S_AXI_HP0_DATA_WIDTH_G   => S_AXI_HP0_DATA_WIDTH_G,
            S_AXI_HP0_ID_WIDTH_G     => S_AXI_HP0_ID_WIDTH_G,
            S_AXI_HP0_AXLEN_WIDTH_G  => S_AXI_HP0_AXLEN_WIDTH_G,
            S_AXI_HP0_AXLOCK_WIDTH_G => S_AXI_HP0_AXLOCK_WIDTH_G,
            --
            S_AXI_HP1_ADDR_WIDTH_G   => S_AXI_HP1_ADDR_WIDTH_G,
            S_AXI_HP1_DATA_WIDTH_G   => S_AXI_HP1_DATA_WIDTH_G,
            S_AXI_HP1_ID_WIDTH_G     => S_AXI_HP1_ID_WIDTH_G,
            S_AXI_HP1_AXLEN_WIDTH_G  => S_AXI_HP1_AXLEN_WIDTH_G,
            S_AXI_HP1_AXLOCK_WIDTH_G => S_AXI_HP1_AXLOCK_WIDTH_G,
            --
            S_AXI_HP2_ADDR_WIDTH_G   => S_AXI_HP2_ADDR_WIDTH_G,
            S_AXI_HP2_DATA_WIDTH_G   => S_AXI_HP2_DATA_WIDTH_G,
            S_AXI_HP2_ID_WIDTH_G     => S_AXI_HP2_ID_WIDTH_G,
            S_AXI_HP2_AXLEN_WIDTH_G  => S_AXI_HP2_AXLEN_WIDTH_G,
            S_AXI_HP2_AXLOCK_WIDTH_G => S_AXI_HP2_AXLOCK_WIDTH_G,
            --
            S_AXI_HP3_ADDR_WIDTH_G   => S_AXI_HP3_ADDR_WIDTH_G,
            S_AXI_HP3_DATA_WIDTH_G   => S_AXI_HP3_DATA_WIDTH_G,
            S_AXI_HP3_ID_WIDTH_G     => S_AXI_HP3_ID_WIDTH_G,
            S_AXI_HP3_AXLEN_WIDTH_G  => S_AXI_HP3_AXLEN_WIDTH_G,
            S_AXI_HP3_AXLOCK_WIDTH_G => S_AXI_HP3_AXLOCK_WIDTH_G)
        port map (
            --__CONNECT_FCLK_CLK0__
            fclk_clk0         => FCLK_CLK0,            --__CONNECT_FCLK_CLK1__
            fclk_clk1         => FCLK_CLK1,
            --__CONNECT_M_AXI_GP0__
            m_axi_gp0_aclk    => M_AXI_GP0_ACLK,
            m_axi_gp0_aresetn => m_axi_gp0_aresetn_s,
            m_axi_gp0_awvalid => M_AXI_GP0_AWVALID,
            m_axi_gp0_awready => M_AXI_GP0_AWREADY,
            m_axi_gp0_awaddr  => M_AXI_GP0_AWADDR,
            m_axi_gp0_awprot  => M_AXI_GP0_AWPROT,
            m_axi_gp0_awqos   => M_AXI_GP0_AWQOS,
            m_axi_gp0_awcache => M_AXI_GP0_AWCACHE,
            m_axi_gp0_awburst => M_AXI_GP0_AWBURST,
            m_axi_gp0_awsize  => M_AXI_GP0_AWSIZE,
            m_axi_gp0_awlen   => M_AXI_GP0_AWLEN,
            m_axi_gp0_awid    => M_AXI_GP0_AWID,
            m_axi_gp0_awlock  => M_AXI_GP0_AWLOCK,
            m_axi_gp0_wid     => M_AXI_GP0_WID,
            m_axi_gp0_wvalid  => M_AXI_GP0_WVALID,
            m_axi_gp0_wready  => M_AXI_GP0_WREADY,
            m_axi_gp0_wdata   => M_AXI_GP0_WDATA,
            m_axi_gp0_wstrb   => M_AXI_GP0_WSTRB,
            m_axi_gp0_wlast   => M_AXI_GP0_WLAST,
            m_axi_gp0_bvalid  => M_AXI_GP0_BVALID,
            m_axi_gp0_bready  => M_AXI_GP0_BREADY,
            m_axi_gp0_bresp   => M_AXI_GP0_BRESP,
            m_axi_gp0_bid     => M_AXI_GP0_BID,
            m_axi_gp0_arvalid => M_AXI_GP0_ARVALID,
            m_axi_gp0_arready => M_AXI_GP0_ARREADY,
            m_axi_gp0_araddr  => M_AXI_GP0_ARADDR,
            m_axi_gp0_arprot  => M_AXI_GP0_ARPROT,
            m_axi_gp0_arqos   => M_AXI_GP0_ARQOS,
            m_axi_gp0_arcache => M_AXI_GP0_ARCACHE,
            m_axi_gp0_arburst => M_AXI_GP0_ARBURST,
            m_axi_gp0_arsize  => M_AXI_GP0_ARSIZE,
            m_axi_gp0_arlen   => M_AXI_GP0_ARLEN,
            m_axi_gp0_arid    => M_AXI_GP0_ARID,
            m_axi_gp0_arlock  => M_AXI_GP0_ARLOCK,
            m_axi_gp0_rvalid  => M_AXI_GP0_RVALID,
            m_axi_gp0_rready  => M_AXI_GP0_RREADY,
            m_axi_gp0_rdata   => M_AXI_GP0_RDATA,
            m_axi_gp0_rresp   => M_AXI_GP0_RRESP,
            m_axi_gp0_rid     => M_AXI_GP0_RID,
            m_axi_gp0_rlast   => M_AXI_GP0_RLAST,
            --__
            --
            rst_o             => rst_s);

    FCLK_RESET0_N <= not rst_s;

    --
    m_axi_gp0_aresetn_s <= not rst_s;
    m_axi_gp1_aresetn_s <= not rst_s;
    --
    s_axi_gp0_aresetn_s <= not rst_s;
    s_axi_gp1_aresetn_s <= not rst_s;
    --
    s_axi_hp0_aresetn_s <= not rst_s;
    s_axi_hp1_aresetn_s <= not rst_s;
    s_axi_hp2_aresetn_s <= not rst_s;
    s_axi_hp3_aresetn_s <= not rst_s;
    --

    -- Other inputs and outputs are left unconnected because they are not
    -- needed for this design and will therefore stay in an 'U' or 'X' state in
    -- simulation. They should not connected to anything meaningful in the top
    -- design.

end architecture struct;