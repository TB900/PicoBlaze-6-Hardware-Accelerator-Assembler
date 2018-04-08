library ieee;
use ieee.std_logic_1164.all;

entity HACC_tb is
end HACC_tb;

architecture test of HACC_tb is
	component HACC
		port
		(
			CLK, START, MEM_GNT : in std_logic;
			DATA_IN : in std_logic_vector(7 downto 0);
			DATA_OUT, ADDR : out std_logic_vector(7 downto 0);
			MEM_RQ, RD_RQ, WR_RQ, PB_SLEEP : out std_logic;
			ST_OUT : out std_logic_vector(2 downto 0)
		);
	end component;

	component dpram
		port
		(
			data_a, data_b, addr_a, addr_b : in std_logic_vector(7 downto 0);
			re_b, we_a, we_b, clk : in std_logic;
			q_a, q_b : out std_logic_vector(7 downto 0)
		);
	end component;

	signal DATA_IN, DATA_OUT, ADDR : std_logic_vector(7 downto 0);
	signal MEM_GNT, START, CLK, RD_RQ, WR_RQ, PB_SLEEP, MEM_RQ : std_logic;
	signal ST_OUT : std_logic_vector(2 downto 0);

	signal data_a, addr_a, q_a : std_logic_vector(7 downto 0);
	signal we_a : std_logic;

	signal max_cycles : integer := 5000;
	signal cycle_count : integer := 1;

begin

	HACC_HA : HACC port map (CLK => CLK, MEM_RQ => MEM_RQ, MEM_GNT => MEM_GNT, START => START, PB_SLEEP => PB_SLEEP, ST_OUT => ST_OUT, RD_RQ => RD_RQ, WR_RQ => WR_RQADDR => ADDR, DATA_IN => DATA_IN, DATA_OUT => DATA_OUT);
	MEM : dpram port map (data_a => data_a, data_b => DATA_OUT, addr_a => addr_a, addr_b => ADDR,re_b => RD_RQ, we_a => we_a, we_b => WR_RQ, clk => CLK, q_a => q_a, q_b => DATA_IN);

	CLK_ticks : process
	begin
		while cycle_count < max_cycles loop
			CLK <= '1';
			wait for 2.5 ns;
			CLK <= '0';
			wait for 2.5 ns;
			cycle_count <= cycle_count + 1;
		end loop;
	end process;

	Inputs: process
	begin
		START <= '1' after 0 ns, '0' after 10 ns;
		while cycle_count < max_cycles loop
			MEM_GNT <= '1';
			wait for 5ns;
			MEM_GNT <= '0';
			wait for 5ns;
		end loop;
	end process;
end test;