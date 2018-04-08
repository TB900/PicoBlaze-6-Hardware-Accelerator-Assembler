library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity HACC is
	port
		(
			CLK, START, MEM_GNT : in std_logic;
			DATA_IN : in std_logic_vector(7 downto 0);
			DATA_OUT, ADDR : out std_logic_vector(7 downto 0);
			MEM_RQ, RD_RQ, WR_RQ, PB_SLEEP : out std_logic;
			ST_OUT : out std_logic_vector(2 downto 0)
		);
end HACC;

architecture HACC_arch of HACC is
	type state_type is (ST_START, ST0, WS0, ST1, WS1, ST_END);
	signal PS : state_type;
begin
	sync_proc : process(CLK, PS, START, MEM_GNT)
	begin
		if (START = '1') then
			PB_SLEEP <= '1';
		end if;

		if (rising_edge(CLK)) then
			case PS is
				when ST_START =>
					if (START = '1') then
						PS <= ST0;
					else 
						PB_SLEEP <= '0';
						PS <= ST_START;
					end if;

				when ST0 =>
					MEM_RQ <= '1';
					if (MEM_GNT = '1') then
						MEM_RQ <= '0';
						PS <= WS0;
					end if;

				when WS0 =>
					RD_RQ <= '1';
					if (MEM_GNT = '0') then
						RD_RQ <= '0';
						PS <= ST1;
					end if;

				when ST1 =>
					MEM_RQ <= '1';
					if (MEM_GNT = '1') then
						MEM_RQ <= '0';
						PS <= WS1;
					end if;

				when WS1 =>
					WR_RQ <= '1';
					if (MEM_GNT = '0') then
						WR_RQ <= '0';
						PS <= ST_END;
					end if;

				when ST_END =>
					PB_SLEEP <= '0';
					PS <= ST_START;

				when others =>
					PS <= ST_START;

			end case;
		end if;
	end process sync_proc;

	action_proc : process(PS)
		variable s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sA, sB, sC, sD, sE, sF : unsigned(7 downto 0);
		variable TMP : unsigned(8 downto 0);
		variable CARRY, ZERO : std_logic;
	begin
		case (PS) is
			when ST0 =>
				-- INPUT s0, 00
			when WS0 =>
				ADDR <= x"00";

			when ST1 =>
				s0 := unsigned(DATA_IN);
				if (s0 = "00000000") then
					ZERO := '1';
				else 
					ZERO := '0';
				end if;

				-- ADD s0, 05
				TMP := ('0' & s0) + x"05";
				s0 := TMP(7 downto 0);
				CARRY := TMP(8);
				if (s0 = "00000000") then
					ZERO := '1';
				else 
					ZERO := '0';
				end if;

				-- OUTPUT s0, 00
			when WS1 =>
				ADDR <= x"00";
				DATA_OUT <= std_logic_vector(s0);

			when ST_END =>
			when others =>
				null;

		end case;
	end process action_proc;

	with PS select
		ST_OUT <= "000" when ST_START,
			 "001" when ST0,
			 "010" when WS0,
			 "011" when ST1,
			 "100" when WS1,
			 "101" when ST_END,
			 "000" when others;
end HACC_arch;