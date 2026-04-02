library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

-- Entity declaration: This version has no input ports as clock and reset are internal.
entity gpiotb is

end gpiotb;

architecture Behavioral of gpiotb is
    -- Internal signals to replace hardware input pins
    signal internal_clk   : std_logic := '0';
    signal internal_reset : std_logic := '1'; -- Start with reset active (High)

    -- Counter registers
    signal temp_count : unsigned(7 downto 0) := (others => '0');
    signal direction  : std_logic := '0'; -- 0: Count Up, 1: Count Down

    -- Timing constants for simulation
    constant CLK_PERIOD : time := 1000000000 ns;
	alias target_sig is <<signal .design_1_wrapper.switch_8bits_tri_i : std_ulogic_vector>>;

begin

    -----------------------------------------------------------
    -- 1. INTERNAL CLOCK GENERATOR
    -- This process creates a toggle every half period to simulate a clock.
    -- Note: This is a simulation construct and is not synthesizable for FPGA.
    -----------------------------------------------------------
    clk_gen : process
    begin
        --while now < 2000 ns loop -- Run simulation for 2 microseconds
        loop
            internal_clk <= '0';
            wait for CLK_PERIOD / 2;
            internal_clk <= '1';
            wait for CLK_PERIOD / 2;
        end loop;
        wait; -- Stop clock generation after the time limit
    end process;

    -----------------------------------------------------------
    -- 2. INTERNAL RESET GENERATOR (Power-On Reset)
    -- Simulates a hardware reset pulse that occurs once at startup.
    -----------------------------------------------------------
    reset_gen : process
    begin
        internal_reset <= '1';    -- Hold system in reset
        wait for 5000 ms;           -- Wait for 2.5 clock cycles
        internal_reset <= '0';    -- Release reset to start the counter
        wait;                     -- Suspend this process indefinitely
    end process;

    -----------------------------------------------------------
    -- 3. UP-DOWN COUNTER LOGIC
    -- Triggered by the rising edge of the internal clock.
    -----------------------------------------------------------
    counter_logic : process(internal_clk, internal_reset)
    begin
        -- Asynchronous Reset handling
        if internal_reset = '1' then
            temp_count <= (others => '0');
            direction  <= '0'; -- Always start by counting up

        elsif rising_edge(internal_clk) then
            -- Directional Logic
            if direction = '0' then
                -- MODE: COUNTING UP
                if temp_count = 255 then
                    direction <= '1';      -- Switch to Down mode upon reaching Max
                    temp_count <= temp_count - 1;
                else
                    temp_count <= temp_count + 1;
                end if;
            else
                -- MODE: COUNTING DOWN
                if temp_count = 0 then
                    direction <= '0';      -- Switch to Up mode upon reaching Min
                    temp_count <= temp_count + 1;
                else
                    temp_count <= temp_count - 1;
                end if;
            end if;
        end if;
    end process;

    -- Concurrent assignment: Convert internal unsigned value to external vector
    target_sig <= std_logic_vector(temp_count);

end Behavioral;
