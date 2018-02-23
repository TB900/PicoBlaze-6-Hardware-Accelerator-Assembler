library ieee;
use ieee.std_logic_1164.all;

entity HACC_tb is
end HACC_tb;

architecture test of HACC_tb is
	component my_fsm
		port
		(
			MEM_GNT, START, CLK : in std_logic;
			RD_RQ, WR_RQ : out std_logic;
			Y : out std_logic_vector(3 downto 0)
		);
	end component;

	signal MEM_GNT, START, CLK, RD_RQ, WR_RQ : std_logic;
	signal Y : std_logic_vector(3 downto 0);
	signal begin
	FSM : my_fsm port map (MEM_GNT => MEM_GNT, START => START, CLK => CLK, Y => Y, RD_RQ => RD_RQ, WR_RQ => WR_RQ);

	CLK_ticks : process
	begin
		loop
			CLK <= '1';
			wait for 2.5 ns;
			CLK <= '0';
			wait for 2.5 ns;
		end loop;
	end process;

	Inputs: process
	begin
		START <= '1' after 0 ns;
		MEM_GNT <= '0' after 0 ns,
		'1' after 10 ns,
		'0' after 20 ns,
		'1' after 30 ns,
		'0' after 40 ns,
		'1' after 50 ns,
		'0' after 60 ns,
		'1' after 70 ns,
		'0' after 80 ns,
		'1' after 90 ns,
		'0' after 100 ns,
		'1' after 110 ns,
		'0' after 120 ns;
		
		assert false report "Reached end of text";
		wait;

	end process;
end test;