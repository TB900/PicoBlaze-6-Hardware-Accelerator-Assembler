library ieee;
use ieee.std_logic_1164.all;

entity my_fsm is
	port
		(
			WAIT_SIG, CLK, RESET : in std_logic;

			IN0 : in std_logic_vector(31 downto 0);
			IN1 : in std_logic_vector(31 downto 0);

			OUT0 : out std_logic_vector(31 downto 0);

			RD, WR : out std_logic_vector(31 downto 0);
			Y : out std_logic_vector(2 downto 0)
		);
end my_fsm;

architecture fsm_arch of my_fsm is
	type state_type is (ST0, WS0, ST1, WS1, ST2, WS2, ST3, WS3);
	signal PS, NS : state_type;
begin
	sync_proc : process(CLK, NS, RESET)
	begin
		if (RESET = '1') then PS <= ST0;
		elsif (rising_edge(CLK)) then
			case PS is
				when ST0 =>
					if (WAIT_SIG = '1') then
						PS <= WS0;
					end if;
				when WS0 =>
					if (WAIT_SIG = '0') then
						PS <= ST1;
					end if;
				when ST1 =>
					if (WAIT_SIG = '1') then
						PS <= WS1;
					end if;
				when WS1 =>
					if (WAIT_SIG = '0') then
						PS <= ST2;
					end if;
				when ST2 =>
					if (WAIT_SIG = '1') then
						PS <= WS2;
					end if;
				when WS2 =>
					if (WAIT_SIG = '0') then
						PS <= ST3;
					end if;
				when ST3 =>
					if (WAIT_SIG = '1') then
						PS <= WS3;
					end if;
				when WS3 =>
					if (WAIT_SIG = '0') then
						PS <= ST0;
					end if;
				when others =>
					 PS <= ST0;
			end case;
		end if;
	end process sync_proc;

	with PS select
		Y <= "000" when ST0,
			 "001" when WS0,
			 "010" when ST1,
			 "011" when WS1,
			 "100" when ST2,
			 "101" when WS2,
			 "110" when ST3,
			 "111" when WS3,
			 "000" when others;
end fsm_arch;