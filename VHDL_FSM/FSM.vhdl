library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity HACC is
	port
		(
			CLK, RESET, MEM_GNT : in std_logic;
			DATA_IN : in std_logic_vector(7 downto 0);
			DATA_OUT, ADDR : out std_logic_vector(7 downto 0);
			MEM_RQ, RD_RQ, WR_RQ : out std_logic;
			Y : out std_logic_vector(1 downto 0)
		);
end HACC;

architecture HACC_arch of HACC is
	type state_type is (ST_START, ST_END);
	signal PS : state_type;
begin
	sync_proc : process(CLK, PS, RESET, MEM_GNT)
	begin
		if (RESET = '1') then PS <= ST_START;
		elsif (rising_edge(CLK)) then
			case PS is
				when ST_START =>
					PS <= ST0;
				when ST_END =>
					PS <= ST_END;
				when others =>
					PS <= ST_INIT;
			end case;
		end if;
	end process sync_proc;

	with PS select
		Y <= "00" when ST_START,
			 "01" when ST_END,
			 "00" when others;
end HACC_arch;