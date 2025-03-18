import sys
import re

header = """-------------------------------------------------------------------------------
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
"""

generics = """
entity __BD_PROCESSING_SYSTEM7_0_0_NAME__ is
    generic(
        QEMU_PATH_TO_SOCKET_G    : string  := "unix:/path";
        QEMU_SYNC_QUANTUM_G      : integer := 10000;
        --
        FCLK_CLK0_PERIOD_IN_NS_G : integer := __FCLK_CLK0_PERIOD_IN_NS__;
        FCLK_CLK1_PERIOD_IN_NS_G : integer := __FCLK_CLK1_PERIOD_IN_NS__;
        FCLK_CLK2_PERIOD_IN_NS_G : integer := __FCLK_CLK2_PERIOD_IN_NS__;
        FCLK_CLK3_PERIOD_IN_NS_G : integer := __FCLK_CLK3_PERIOD_IN_NS__;
        --
        M_AXI_GP0_ENABLE_G       : integer := __M_AXI_GP0_ENABLE__;
        M_AXI_GP1_ENABLE_G       : integer := __M_AXI_GP1_ENABLE__;
        --
        S_AXI_GP0_ENABLE_G       : integer := __S_AXI_GP0_ENABLE__;
        S_AXI_GP1_ENABLE_G       : integer := __S_AXI_GP1_ENABLE__;
        S_AXI_HP0_ENABLE_G       : integer := __S_AXI_HP0_ENABLE__;
        S_AXI_HP1_ENABLE_G       : integer := __S_AXI_HP1_ENABLE__;
        S_AXI_HP2_ENABLE_G       : integer := __S_AXI_HP2_ENABLE__;
        S_AXI_HP3_ENABLE_G       : integer := __S_AXI_HP3_ENABLE__;
        --
        M_AXI_GP0_ADDR_WIDTH_G   : integer := __M_AXI_GP0_ADDR_WIDTH__;
        M_AXI_GP0_DATA_WIDTH_G   : integer := __M_AXI_GP0_DATA_WIDTH__;
        M_AXI_GP0_ID_WIDTH_G     : integer := __M_AXI_GP0_ID_WIDTH__;
        M_AXI_GP0_AXLEN_WIDTH_G  : integer := __M_AXI_GP0_AXLEN_WIDTH__;
        M_AXI_GP0_AXLOCK_WIDTH_G : integer := __M_AXI_GP0_AXLOCK_WIDTH__;
        --
        M_AXI_GP1_ADDR_WIDTH_G   : integer := __M_AXI_GP1_ADDR_WIDTH__;
        M_AXI_GP1_DATA_WIDTH_G   : integer := __M_AXI_GP1_DATA_WIDTH__;
        M_AXI_GP1_ID_WIDTH_G     : integer := __M_AXI_GP1_ID_WIDTH__;
        M_AXI_GP1_AXLEN_WIDTH_G  : integer := __M_AXI_GP1_AXLEN_WIDTH__;
        M_AXI_GP1_AXLOCK_WIDTH_G : integer := __M_AXI_GP1_AXLOCK_WIDTH__;
        --
        S_AXI_GP0_ADDR_WIDTH_G   : integer := __S_AXI_GP0_ADDR_WIDTH__;
        S_AXI_GP0_DATA_WIDTH_G   : integer := __S_AXI_GP0_DATA_WIDTH__;
        S_AXI_GP0_ID_WIDTH_G     : integer := __S_AXI_GP0_ID_WIDTH__;
        S_AXI_GP0_AXLEN_WIDTH_G  : integer := __S_AXI_GP0_AXLEN_WIDTH__;
        S_AXI_GP0_AXLOCK_WIDTH_G : integer := __S_AXI_GP0_AXLOCK_WIDTH__;
        --
        S_AXI_GP1_ADDR_WIDTH_G   : integer := __S_AXI_GP1_ADDR_WIDTH__;
        S_AXI_GP1_DATA_WIDTH_G   : integer := __S_AXI_GP1_DATA_WIDTH__;
        S_AXI_GP1_ID_WIDTH_G     : integer := __S_AXI_GP1_ID_WIDTH__;
        S_AXI_GP1_AXLEN_WIDTH_G  : integer := __S_AXI_GP1_AXLEN_WIDTH__;
        S_AXI_GP1_AXLOCK_WIDTH_G : integer := __S_AXI_GP1_AXLOCK_WIDTH__;
        --
        S_AXI_HP0_ADDR_WIDTH_G   : integer := __S_AXI_HP0_ADDR_WIDTH__;
        S_AXI_HP0_DATA_WIDTH_G   : integer := __S_AXI_HP0_DATA_WIDTH__;
        S_AXI_HP0_ID_WIDTH_G     : integer := __S_AXI_HP0_ID_WIDTH__;
        S_AXI_HP0_AXLEN_WIDTH_G  : integer := __S_AXI_HP0_AXLEN_WIDTH__;
        S_AXI_HP0_AXLOCK_WIDTH_G : integer := __S_AXI_HP0_AXLOCK_WIDTH__;
        --
        S_AXI_HP1_ADDR_WIDTH_G   : integer := __S_AXI_HP1_ADDR_WIDTH__;
        S_AXI_HP1_DATA_WIDTH_G   : integer := __S_AXI_HP1_DATA_WIDTH__;
        S_AXI_HP1_ID_WIDTH_G     : integer := __S_AXI_HP1_ID_WIDTH__;
        S_AXI_HP1_AXLEN_WIDTH_G  : integer := __S_AXI_HP1_AXLEN_WIDTH__;
        S_AXI_HP1_AXLOCK_WIDTH_G : integer := __S_AXI_HP1_AXLOCK_WIDTH__;
        --
        S_AXI_HP2_ADDR_WIDTH_G   : integer := __S_AXI_HP2_ADDR_WIDTH__;
        S_AXI_HP2_DATA_WIDTH_G   : integer := __S_AXI_HP2_DATA_WIDTH__;
        S_AXI_HP2_ID_WIDTH_G     : integer := __S_AXI_HP2_ID_WIDTH__;
        S_AXI_HP2_AXLEN_WIDTH_G  : integer := __S_AXI_HP2_AXLEN_WIDTH__;
        S_AXI_HP2_AXLOCK_WIDTH_G : integer := __S_AXI_HP2_AXLOCK_WIDTH__;
        --
        S_AXI_HP3_ADDR_WIDTH_G   : integer := __S_AXI_HP3_ADDR_WIDTH__;
        S_AXI_HP3_DATA_WIDTH_G   : integer := __S_AXI_HP3_DATA_WIDTH__;
        S_AXI_HP3_ID_WIDTH_G     : integer := __S_AXI_HP3_ID_WIDTH__;
        S_AXI_HP3_AXLEN_WIDTH_G  : integer := __S_AXI_HP3_AXLEN_WIDTH__;
        S_AXI_HP3_AXLOCK_WIDTH_G : integer := __S_AXI_HP3_AXLOCK_WIDTH__
        );
"""

architecture = """
end entity __BD_PROCESSING_SYSTEM7_0_0_NAME__;

architecture struct of __BD_PROCESSING_SYSTEM7_0_0_NAME__ is

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
            S_AXI_HP3_AXLOCK_WIDTH_G => S_AXI_HP3_AXLOCK_WIDTH_G)"""

port_map = """
        port map (
"""

end_architecture = """
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

end architecture struct;"""

clock_connection = """            --__CONNECT_FCLK_CLKX__
            fclk_clkX         => FCLK_CLKX,"""

axi_connection = """
            --__CONNECT_AXIU__
            AXIL_aclk    => AXIU_ACLK,
            AXIL_aresetn => AXIL_aresetn_s,
            AXIL_awvalid => AXIU_AWVALID,
            AXIL_awready => AXIU_AWREADY,
            AXIL_awaddr  => AXIU_AWADDR,
            AXIL_awprot  => AXIU_AWPROT,
            AXIL_awqos   => AXIU_AWQOS,
            AXIL_awcache => AXIU_AWCACHE,
            AXIL_awburst => AXIU_AWBURST,
            AXIL_awsize  => AXIU_AWSIZE,
            AXIL_awlen   => AXIU_AWLEN,
            AXIL_awid    => AXIU_AWID,
            AXIL_awlock  => AXIU_AWLOCK,
            AXIL_wid     => AXIU_WID,
            AXIL_wvalid  => AXIU_WVALID,
            AXIL_wready  => AXIU_WREADY,
            AXIL_wdata   => AXIU_WDATA,
            AXIL_wstrb   => AXIU_WSTRB,
            AXIL_wlast   => AXIU_WLAST,
            AXIL_bvalid  => AXIU_BVALID,
            AXIL_bready  => AXIU_BREADY,
            AXIL_bresp   => AXIU_BRESP,
            AXIL_bid     => AXIU_BID,
            AXIL_arvalid => AXIU_ARVALID,
            AXIL_arready => AXIU_ARREADY,
            AXIL_araddr  => AXIU_ARADDR,
            AXIL_arprot  => AXIU_ARPROT,
            AXIL_arqos   => AXIU_ARQOS,
            AXIL_arcache => AXIU_ARCACHE,
            AXIL_arburst => AXIU_ARBURST,
            AXIL_arsize  => AXIU_ARSIZE,
            AXIL_arlen   => AXIU_ARLEN,
            AXIL_arid    => AXIU_ARID,
            AXIL_arlock  => AXIU_ARLOCK,
            AXIL_rvalid  => AXIU_RVALID,
            AXIL_rready  => AXIU_RREADY,
            AXIL_rdata   => AXIU_RDATA,
            AXIL_rresp   => AXIU_RRESP,
            AXIL_rid     => AXIU_RID,
            AXIL_rlast   => AXIU_RLAST,"""

irq_connection = """
            --__CONNECT_IRQ_F2P__
            irq_f2p           => std_logic_vector(resize(unsigned(IRQ_F2P), 16)),"""

end_port_map = """
            --__
            --
            rst_o             => rst_s);

"""

clock_port_pattern = re.compile("FCLK_CLK[0-3]")
axi_port_pattern = re.compile("[MS]_AXI_[GH]P[0-3]")

class DefaultAXIParameters:
    """Default AXI Parameters"""
    def __init__(self):
        # Default values
        self.addr_width   = 32
        self.data_width   = 32
        self.id_width     = 12
        self.axlen_width  = 4
        self.axlock_width = 2
        
class DefaultHPAXIParameters:
    """Default AXI Parameters"""
    def __init__(self):
        # Default values
        self.addr_width   = 32
        self.data_width   = 64
        self.id_width     = 6
        self.axlen_width  = 4
        self.axlock_width = 2

class AXIParameters:
    """AXI Parameters"""
    
    def __init__(self, fileName, axi_port):
        extract_pattern = "\s*:\s*(?:in|out)\s+.*\(\s+([0-9]+)\s+downto.*"
        self.name = axi_port
        
        if(re.match(axi_port_pattern, axi_port)):
            #print("Port is acceptable")
            addr_pattern   = re.compile(".*" + axi_port + "_ARADDR" + extract_pattern)
            data_pattern   = re.compile(".*" + axi_port + "_RDATA" + extract_pattern)
            id_pattern     = re.compile(".*" + axi_port + "_RID" + extract_pattern)
            axlen_pattern  = re.compile(".*" + axi_port + "_ARLEN" + extract_pattern)
            axlock_pattern = re.compile(".*" + axi_port + "_ARLOCK" + extract_pattern)
            
            with open(fileName) as file:
                f = file.read()
                self.addr_width = int(re.search(addr_pattern, f).group(1)) + 1
                self.data_width = int(re.search(data_pattern, f).group(1)) + 1
                self.id_width = int(re.search(id_pattern, f).group(1)) + 1
                self.axlen_width = int(re.search(axlen_pattern, f).group(1)) + 1
                self.axlock_width = int(re.search(axlock_pattern, f).group(1)) + 1
                
        else:
            print(axi_port)
            print("Port does not seem to be an AXI port")


    def getAxiDefineString(self):
        return " -D__" + self.name + "_ENABLE__=1" + \
        " -D__" + self.name + "_ADDR_WIDTH__=" + str(self.addr_width) + \
        " -D__" + self.name + "_DATA_WIDTH__=" + str(self.data_width) + \
        " -D__" + self.name + "_ID_WIDTH__=" + str(self.id_width) + \
        " -D__" + self.name + "_AXLEN_WIDTH__=" + str(self.axlen_width) + \
        " -D__" + self.name + "_AXLOCK_WIDTH__=" + str(self.axlock_width)
        

def axiPortConnection(axi_port):
    if(re.match(axi_port_pattern, axi_port)):
        axil = axi_port.lower()
        axiu = axi_port.upper()
        axi_con = axi_connection
        axi_con = axi_con.replace("AXIL", axil)
        axi_con = axi_con.replace("AXIU", axiu)
        return axi_con
    else:
        print(axi_port)
        print("Port does not seem to be an AXI port")
        return ""

def clockConnection(clock):
    clock_con = clock_connection.replace("X", str(clock))
    return clock_con
            
def getName(fileStr):
    extract_pattern = "\s*entity\s+(.*7_0_0)\s+is.*"
    extract_pattern1 = "\s*entity\s+(.*7_0)\s+is.*"
    res0_0 = re.search(extract_pattern, fileStr)
    res0 = re.search(extract_pattern1, fileStr)
    if res0_0:
        return res0_0.group(1)
    elif res0:
        return res0.group(1)
    

def getNameFromFile(fileName):
    with open(fileName) as file:
        f = file.read()
        return getName(f)

class ProcessingSystem7:
    """Represents the Processing System being parsed"""

    
    def __init__(self, fileName):
        with open(fileName) as file:
            f = file.read()

            # Extract the name
            self.name = getName(f)
            f = read_ports_between_marks(fileName)
            # Extract clocks
            if ("FCLK_CLK0" in f):
                self.has_clock_0 = True
            else:
                self.has_clock_0 = False
            if ("FCLK_CLK1" in f):
                self.has_clock_1 = True
            else:
                self.has_clock_1 = False
            if ("FCLK_CLK2" in f):
                self.has_clock_2 = True
            else:
                self.has_clock_2 = False
            if ("FCLK_CLK3" in f):
                self.has_clock_3 = True
            else:
                self.has_clock_3 = False

            self.clock_ports = set(re.findall(clock_port_pattern, f))
            self.axi_ports = set(re.findall(axi_port_pattern, f))

            if ("IRQ_F2P" in f):
                self.has_irq_f2p = True
            else:
                self.has_irq_f2p = False

def replaceUnParameterizedAXIPorts(fileStrToUpdate):
    default_params = DefaultAXIParameters()
    default_hp_params = DefaultHPAXIParameters()

    output = re.sub("__[MS]_AXI_GP[0-1]_ADDR_WIDTH__", str(default_params.addr_width), fileStrToUpdate)
    output = re.sub("__[MS]_AXI_GP[0-1]_DATA_WIDTH__", str(default_params.data_width), output)
    output = re.sub("__[M]_AXI_GP[0-1]_ID_WIDTH__", str(default_params.id_width), output)
    output = re.sub("__[S]_AXI_GP[0-1]_ID_WIDTH__", str(6), output) # For some reason this is different
    output = re.sub("__[MS]_AXI_GP[0-1]_AXLEN_WIDTH__", str(default_params.axlen_width), output)
    output = re.sub("__[MS]_AXI_GP[0-1]_AXLOCK_WIDTH__", str(default_params.axlock_width), output)

    output = re.sub("__[MS]_AXI_HP[0-3]_ADDR_WIDTH__", str(default_hp_params.addr_width), output)
    output = re.sub("__[MS]_AXI_HP[0-3]_DATA_WIDTH__", str(default_hp_params.data_width), output)
    output = re.sub("__[MS]_AXI_HP[0-3]_ID_WIDTH__", str(default_hp_params.id_width), output)
    output = re.sub("__[MS]_AXI_HP[0-3]_AXLEN_WIDTH__", str(default_hp_params.axlen_width), output)
    output = re.sub("__[MS]_AXI_HP[0-3]_AXLOCK_WIDTH__", str(default_hp_params.axlock_width), output)
    
    output = re.sub("__[MS]_AXI_[GH]P[0-3]_ENABLE__", "0", output)
    
    return output
                
def applyAXIParameters(inputFile, fileStrToUpdate):
    ps = ProcessingSystem7(inputFile)

    output = fileStrToUpdate
    
    for axi_p in ps.axi_ports:
        axi_param = AXIParameters(inputFile, axi_p)
        output = re.sub("__" + axi_p + "_ADDR_WIDTH__", str(axi_param.addr_width), output)
        output = re.sub("__" + axi_p + "_DATA_WIDTH__", str(axi_param.data_width), output)
        output = re.sub("__" + axi_p + "_ID_WIDTH__", str(axi_param.id_width), output)
        output = re.sub("__" + axi_p + "_AXLEN_WIDTH__", str(axi_param.axlen_width), output)
        output = re.sub("__" + axi_p + "_AXLOCK_WIDTH__", str(axi_param.axlock_width), output)

        output = re.sub("__" + axi_p + "_ENABLE__", "1", output)

    # Replaces all un parameterized generics by default values
    output = replaceUnParameterizedAXIPorts(output)
        
    return output
                
def generateVHDLFileContents(fileName):
    # Create the processing system file
    processing_system = ProcessingSystem7(fileName)

    # Set the name
    updated_generics = generics.replace("__BD_PROCESSING_SYSTEM7_0_0_NAME__", processing_system.name)
    updated_architecture = architecture.replace("__BD_PROCESSING_SYSTEM7_0_0_NAME__", processing_system.name)

    # Set generics
    updated_generics = applyAXIParameters(fileName, updated_generics)

    # Set port map
    updated_port_map = port_map

    updated_end_architecture = end_architecture
    
    # TODO : refactor this, this adds connection and generics for all clocks enabled
    # Wojtek - removed updated_end_archtecture for other than RESET0
    if processing_system.has_clock_0:
        updated_port_map += clockConnection(0)
        updated_generics = updated_generics.replace("__FCLK_CLK0_PERIOD_IN_NS__", str(10000))
        updated_end_architecture = "    FCLK_RESET0_N <= not rst_s;\n" + updated_end_architecture
    else:
        updated_generics = updated_generics.replace("__FCLK_CLK0_PERIOD_IN_NS__", str(-1))
    if processing_system.has_clock_1:
        updated_port_map += clockConnection(1)
        updated_generics = updated_generics.replace("__FCLK_CLK1_PERIOD_IN_NS__", str(10000))
        #updated_end_architecture = "    FCLK_RESET1_N <= not rst_s;\n" + updated_end_architecture
    else:
        updated_generics = updated_generics.replace("__FCLK_CLK1_PERIOD_IN_NS__", str(-1))
    if processing_system.has_clock_2:
        updated_port_map += clockConnection(2)
        updated_generics = updated_generics.replace("__FCLK_CLK2_PERIOD_IN_NS__", str(10000))
        #updated_end_architecture = "    FCLK_RESET2_N <= not rst_s;\n" + updated_end_architecture
    else:
        updated_generics = updated_generics.replace("__FCLK_CLK2_PERIOD_IN_NS__", str(-1))
    if processing_system.has_clock_3:
        updated_port_map += clockConnection(3)
        updated_generics = updated_generics.replace("__FCLK_CLK3_PERIOD_IN_NS__", str(10000))
        #updated_end_architecture = "    FCLK_RESET3_N <= not rst_s;\n" + updated_end_architecture
    else:
        updated_generics = updated_generics.replace("__FCLK_CLK3_PERIOD_IN_NS__", str(-1))
        
    for axi_p in processing_system.axi_ports:
        updated_port_map += axiPortConnection(axi_p)

    if processing_system.has_irq_f2p:
        updated_port_map += irq_connection

    updated_port_map += end_port_map

    # Stitch the output together
    output = header
    output += updated_generics
    output += extractPorts(fileName)
    output += updated_architecture
    output += updated_port_map
    output += updated_end_architecture
        
    return output
    

def extractPorts(fileName):
    name = getNameFromFile(fileName)
    ports = ""
    start = False
    
    for line in open(fileName).read().splitlines():
        if (start == False):
            if (re.match("\s*entity\s+" + name + "\s+is.*", line)):
                start = True
        else:
            ports += line + "\n"
            if (re.match("\s+\);\s*", line)):
                return ports

    return ports


def read_ports_between_marks(file_path):
    port_block = ""
    in_ports_section = False
    line_buffer = []
    closing_parentheses_count = 0
    opening_parentheses_count = 0

    with open(file_path, 'r') as file:
        for line in file:
            if "Port (" in line:
                in_ports_section = True
                opening_parentheses_count += 1
                line_buffer.append(line)
                continue
            if " ( " in line and in_ports_section:
                opening_parentheses_count += 1
            if " );" in line and in_ports_section:
                closing_parentheses_count += 1
                line_buffer.append(line)

                if closing_parentheses_count == opening_parentheses_count:
                    break

            if in_ports_section:
                line_buffer.append(line)

    port_block = "".join(line_buffer)
    return port_block


all_do_script =   """# Main script for Riviera-PRO (auto-generated)
do [TOP_DESIGN]_compile.do
do zynq7_compile_cosim.do
do [TOP_DESIGN]_simulate.do
"""

compile_do_script = """# Script to compile the CoSimulation files (auto-generated)

vlib riviera/work
vmap xil_defaultlib riviera/xil_defaultlib
# Zynq System Wrapper
ccomp -sc -c -o o1.o -g -I./../src/libsystemctlm-soc/libremote-port/ -I./../src/libsystemctlm-soc/ ./../src/libsystemctlm-soc/soc/xilinx/zynq/xilinx-zynq.cc

# Compile C files (not SystemC) for libremote-port
ccomp -sc -c -o o2.o -x c -fPIC -g ./../src/libsystemctlm-soc/libremote-port/safeio.c
# The following file was patched to solve issues (maybe a flag would have fixed them too) TODO : Check this out
ccomp -sc -c -o o3.o -x c -fPIC -g ./../src/libsystemctlm-soc/libremote-port/remote-port-proto.c
# The following file was patched to solve issues
ccomp -sc -c -o o4.o -x c -fPIC -g ./../src/libsystemctlm-soc/libremote-port/remote-port-sk.c

# Lib Remote Port (RP) SystemC files
ccomp -sc -c -o o5.o -g -I./../src/libsystemctlm-soc/libremote-port/ -I./../src/libsystemctlm-soc/ ./../src/libsystemctlm-soc/libremote-port/remote-port-tlm.cc
ccomp -sc -c -o o6.o -g -I./../src/libsystemctlm-soc/libremote-port/ -I./../src/libsystemctlm-soc/ ./../src/libsystemctlm-soc/libremote-port/remote-port-tlm-memory-master.cc
ccomp -sc -c -o o7.o -g -I./../src/libsystemctlm-soc/libremote-port/ -I./../src/libsystemctlm-soc/ ./../src/libsystemctlm-soc/libremote-port/remote-port-tlm-memory-slave.cc
ccomp -sc -c -o o8.o -g -I./../src/libsystemctlm-soc/libremote-port/ -I./../src/libsystemctlm-soc/ ./../src/libsystemctlm-soc/libremote-port/remote-port-tlm-wires.cc

# The main Zynq SystemC-TLM CoSimulation entity
eval ccomp -sc -c -o o9.o -D__QEMU_SYNC_QUANTUM_G__=$env(SYNC_QUANTUM) -D__QEMU_PATH_TO_SOCKET__=$env(RPORT_PATH_SC) -D__M_AXI_GP0_ENABLE__=1 -D__M_AXI_GP0_ADDR_WIDTH__=32 -D__M_AXI_GP0_DATA_WIDTH__=32 -D__M_AXI_GP0_ID_WIDTH__=12 -D__M_AXI_GP0_AXLEN_WIDTH__=4 -D__M_AXI_GP0_AXLOCK_WIDTH__=2 -g -I./../src/libsystemctlm-soc/libremote-port/ -I./../src/libsystemctlm-soc/ -I./../src/libsystemctlm-soc/soc/xilinx/zynq/ -I./../src/src_sc -I. -I./../src/libsystemctlm-soc/tlm-bridges/ ./../src/src_sc/zynq7_ps.cc
ccomp -sc o1.o o2.o o3.o o4.o o5.o o6.o o7.o o8.o o9.o -o zynq7_ps
addsc -work xil_defaultlib zynq7_ps.so

# Compilation of the VHDL wrapper around the SystemC entity (should be consistent with the template above)
vcom -work xil_defaultlib ./../src/src_vhdl/zynq7_ps_wrapper.vhd

# Compile VHDL BD wrapper (auto-generated)
vcom -work xil_defaultlib -2008 ./../src/[BD_WRAPPER_FILE_NAME]

# Simulate (requires QEMU to be launched, twice, once for the optimization and then again for the simulation)
#
# petalinux-boot --qemu --kernel --qemu-args "-hw-dtb ./system.dtb -machine-path ./qemu-tmp -icount 1 -sync-quantum 10000"
# The hardware device tree is the linux device tree with extra include
# https://github.com/Xilinx/qemu-devicetrees
# https://github.com/Xilinx/qemu-devicetrees/blob/master/zynq-pl-remoteport.dtsi
#
# Time quantum and machine path should be the same between simulation and QEMU emulation
# (generic parameters of zynq_top component, can be changed in testbench)
  """

from os import listdir
from os.path import isfile, join

# This deduces the top name from the [top name]_elaborate.do file generated by Vivado
def getTopName(filesInSimDir):
    vivado_elaborate_file = next(x for x in filesInSimDir if re.match(".+_simulate.do", x))
    return vivado_elaborate_file.replace("_simulate.do", "")

def generateCompilationScriptContents(inputFile):
    ps = ProcessingSystem7(inputFile)
    output = compile_do_script.replace("[BD_WRAPPER_FILE_NAME]", ps.name + ".vhd")
    define_string = ""
    for axi_p in ps.axi_ports:
        axi_parameters = AXIParameters(inputFile, axi_p)
        define_string += axi_parameters.getAxiDefineString()

    output = output.replace("[CXX_ARGS_PS7]", define_string)

    return output
        

def generateAllDoFileContents(topName):
    return all_do_script.replace("[TOP_DESIGN]", topName)

def main():
    #print("Argument list: ", str(sys.argv))
    if (len(sys.argv) < 3):
        print("Usage : This program requires two arguments.\nFirst argument is path to processing system VHDL stub file, and second argument is path to simulation directory")
        return

    fileName = sys.argv[1]
    f = open(fileName).read()

    dirPath = sys.argv[2]

    filesInSimDir = [f for f in listdir(dirPath) if isfile(join(dirPath, f))]
    
    if ("system7_0_0" in f):
        print("Could find Processing System in file")
    elif ("sys_ps7" in f):
        print("Could find Processing System in file")
    else:
        print("Could not find processing system in file " + fileName)
        return

    ps = ProcessingSystem7(fileName)
    
    vhdl_file_name = dirPath + "/../src/" + ps.name + ".vhd"
    comp_file_name = dirPath + "/" + "zynq7_compile_cosim.do"
    all_do_file_name = dirPath + "/" + "all.do"
    
    # Generate VHDL File
    vhdl_file_contents = generateVHDLFileContents(fileName)
    output_file = open(vhdl_file_name, "w")
    output_file.write(vhdl_file_contents)
    output_file.close()

    # Generate compilation script
    compile_ps7_file = open(comp_file_name, "w")
    compile_ps7_file.write(generateCompilationScriptContents(fileName))
    compile_ps7_file.close()
    
    # Generate all.do
    all_do_file = open(all_do_file_name, "w")
    all_do_file.write(generateAllDoFileContents(getTopName(filesInSimDir)))
    all_do_file.close()

    print("Genenerated the follwing files : ")
    print(vhdl_file_name)
    print(comp_file_name)
    print(all_do_file_name)
    
main()
