----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 09/15/2021 03:01:00 PM
-- Design Name: 
-- Module Name: sim_top - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity sim_top is
--  Port ( );
end sim_top;

architecture Behavioral of sim_top is
    signal leds_obs : std_logic_vector(7 downto 0);
    signal sw_sti   : std_logic_vector(7 downto 0) := "10100110";
begin
    dut : entity work.design_1_wrapper
    port map (
        PL_user_switches_tri_i  => sw_sti,
        PL_user_LEDs_tri_o => leds_obs
    );

end Behavioral;
