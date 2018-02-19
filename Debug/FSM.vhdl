library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity FSM is
	port
		(
			CLK, RESET, MEM_GNT : in std_logic;
			DATA_IN : in std_logic_vector(7 downto 0);
			DATA_OUT, ADDR : out std_logic_vector(7 downto 0);
			MEM_RQ, RD_RQ, WR_RQ : out std_logic;
			Y : out std_logic_vector(3 downto 0)
		);
end FSM;

architecture HACC_arch of FSM is
	type state_type is (ST_START, ST0, WS0, ST1, WS1, ST2, WS2, ST3, WS3, ST4, WS4, ST5, WS5, ST_END);
	signal PS : state_type;
begin
	sync_proc : process(CLK, PS, RESET, MEM_GNT)
	begin
		if (RESET = '1') then PS <= ST_START;
		elsif (rising_edge(CLK)) then
			case PS is
				when ST_START =>
					PS <= ST0;

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
					RD_RQ <= '1';
					if (MEM_GNT = '0') then
						RD_RQ <= '0';
						PS <= ST2;
					end if;

				when ST2 =>
					MEM_RQ <= '1';
					if (MEM_GNT = '1') then
						MEM_RQ <= '0';
						PS <= WS2;
					end if;

				when WS2 =>
					WR_RQ <= '1';
					if (MEM_GNT = '0') then
						WR_RQ <= '0';
						PS <= ST3;
					end if;

				when ST3 =>
					MEM_RQ <= '1';
					if (MEM_GNT = '1') then
						MEM_RQ <= '0';
						PS <= WS3;
					end if;

				when WS3 =>
					WR_RQ <= '1';
					if (MEM_GNT = '0') then
						WR_RQ <= '0';
						PS <= ST4;
					end if;

				when ST4 =>
					MEM_RQ <= '1';
					if (MEM_GNT = '1') then
						MEM_RQ <= '0';
						PS <= WS4;
					end if;

				when WS4 =>
					WR_RQ <= '1';
					if (MEM_GNT = '0') then
						WR_RQ <= '0';
						PS <= ST5;
					end if;

				when ST5 =>
					MEM_RQ <= '1';
					if (MEM_GNT = '1') then
						MEM_RQ <= '0';
						PS <= WS5;
					end if;

				when WS5 =>
					WR_RQ <= '1';
					if (MEM_GNT = '0') then
						WR_RQ <= '0';
						PS <= ST_END;
					end if;

				when ST_END =>
					PS <= ST_END;

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

				-- FETCH s1, 01
			when WS1 =>
				ADDR <= x"01";

			when ST2 =>
				s1 := unsigned(DATA_IN);
				if (s1 = "00000000") then
					ZERO := '1';
				else 
					ZERO := '0';
				end if;

				-- LOAD s2, 0F
				s2 := x"0F";

				-- LOAD s3, 14
				s3 := x"14";

				-- SR0 s0
				CARRY := s0(0);
				s0 := '0' & s0(7 downto 1);
				if (s0 = "00000000") then
					ZERO := '1';
				else 
					ZERO := '0';
				end if;

				-- SR1 s1
				CARRY := s1(0);
				s1 := '1' & s1(7 downto 1);
				if (s1 = "00000000") then
					ZERO := '1';
				else 
					ZERO := '0';
				end if;

				-- SRA s2
				TMP := CARRY & s2;
				s2 := TMP(8 downto 1);
				CARRY := TMP(0);
				if (s2 = "00000000") then
					ZERO := '1';
				else 
					ZERO := '0';
				end if;

				-- SRX s3
				CARRY := s3(0);
				s3 := s3(7) & s3(7 downto 1);
				if (s3 = "00000000") then
					ZERO := '1';
				else 
					ZERO := '0';
				end if;

				-- OUTPUT s0, 05
			when WS2 =>
				ADDR <= x"05";
				DATA_OUT <= std_logic_vector(s0);

			when ST3 =>
				-- STORE s1, 06
			when WS3 =>
				ADDR <= x"06";
				DATA_OUT <= std_logic_vector(s1);

			when ST4 =>
				-- OUTPUT s2, 07
			when WS4 =>
				ADDR <= x"07";
				DATA_OUT <= std_logic_vector(s2);

			when ST5 =>
				-- STORE s3, 08
			when WS5 =>
				ADDR <= x"08";
				DATA_OUT <= std_logic_vector(s3);

			when ST_END =>
			when others =>
				null;

		end case;
	end process action_proc;

	with PS select
		Y <= "0000" when ST_START,
			 "0001" when ST0,
			 "0010" when WS0,
			 "0011" when ST1,
			 "0100" when WS1,
			 "0101" when ST2,
			 "0110" when WS2,
			 "0111" when ST3,
			 "1000" when WS3,
			 "1001" when ST4,
			 "1010" when WS4,
			 "1011" when ST5,
			 "1100" when WS5,
			 "1101" when ST_END,
			 "0000" when others;
end HACC_arch;