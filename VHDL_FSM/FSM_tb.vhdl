library ieee;
use ieee.std_logic_1164.all;

entity my_fsm_tb is
end my_fsm_tb;

architecture test of my_fsm_tb is
	component my_fsm
		port
		(
			WAIT_SIG, RESET, CLK : in std_logic;
			Z1, Z2 : out std_logic;

			IN0 : in std_logic_vector(31 downto 0);
			IN1 : in std_logic_vector(31 downto 0);

			OUT0 : out std_logic_vector(31 downto 0);

			Y : out std_logic_vector(3 downto 0)
		);
	end component;

	signal WAIT_SIG, RESET, CLK, Z1, Z2 : std_logic;
	signal Y : std_logic_vector(3 downto 0);
	signal IN0, IN1, OUT0 : std_logic_vector(31 downto 0);

begin
	FSM : my_fsm port map (WAIT_SIG => WAIT_SIG, RESET => RESET, CLK => CLK, Z1 => Z1, Z2 => Z2, Y => Y, IN0 => IN0, IN1 => IN1, OUT0 => OUT0);

	CLK_ticks : process
	begin
		loop
			CLK <= '1';
			wait for 5 ns;
			CLK <= '0';
			wait for 5 ns;
		end loop;
	end process;

	Inputs: process
	begin
		WAIT_SIG <= '0' after 0 ns,
		'1' after 10 ns,
		'0' after 20 ns,
		'1' after 30 ns,
		'0' after 40 ns,
		'1' after 50 ns,
		'0' after 60 ns,
		'1' after 70 ns,
		'0' after 80 ns;
		
		assert false report "Reached end of text";
		wait;

	end process;
end test;