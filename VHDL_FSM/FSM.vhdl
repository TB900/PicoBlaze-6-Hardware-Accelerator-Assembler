library ieee;
use ieee.std_logic_1164.all;

entity my_fsm is
	port
		(
			WAIT_SIG, CLK, RESET : in std_logic;

			IN0 : in std_logic_vector(31 downto 0);
			IN1 : in std_logic_vector(31 downto 0);

			OUT0 : out std_logic_vector(31 downto 0);

			Y : out std_logic_vector(3 downto 0);
			Z1, Z2 : out std_logic 
		);
end my_fsm;

architecture fsm_arch of my_fsm is
	type state_type is (ST0, WS0, ST1, WS1, ST2, WS2, ST3, WS3);
	signal PS, NS : state_type;
begin
	sync_proc : process(CLK, NS, RESET)
	begin
		if (RESET = '1') then PS <= ST0;
		elsif (rising_edge(CLK)) then PS <= NS;
		end if;
	end process sync_proc;

	comb_proc : process(PS, WAIT_SIG)
	begin
		Z1 <= '0'; Z2 <= '0';
		case PS is
			when ST0 =>
				Z1 <= '0';
				if (WAIT_SIG = '1') then
					NS <= WS0; Z2 <= '1';
				else
					NS <= ST0; Z2 <= '0';
				end if;
			when WS0 =>
				Z1 <= '1';
				if (WAIT_SIG = '0') then
					NS <= ST1; Z2 <= '0';
				else
					NS <= WS0; Z2 <= '1';
				end if;
			when ST1 =>
				Z1 <= '0';
				if (WAIT_SIG = '1') then
					NS <= WS1; Z2 <= '1';
				else
					NS <= ST1; Z2 <= '0';
				end if;
			when WS1 =>
				Z1 <= '1';
				if (WAIT_SIG = '0') then
					NS <= ST2; Z2 <= '0';
				else
					NS <= WS1; Z2 <= '1';
				end if;
			when ST2 =>
				Z1 <= '0';
				if (WAIT_SIG = '1') then
					NS <= WS2; Z2 <= '1';
				else
					NS <= ST2; Z2 <= '0';
				end if;
			when WS2 =>
				Z1 <= '1';
				if (WAIT_SIG = '0') then
					NS <= ST3; Z2 <= '0';
				else
					NS <= WS2; Z2 <= '1';
				end if;
			when ST3 =>
				Z1 <= '0';
				if (WAIT_SIG = '1') then
					NS <= WS3; Z2 <= '1';
				else
					NS <= ST3; Z2 <= '0';
				end if;
			when WS3 =>
				Z1 <= '1';
				if (WAIT_SIG = '0') then
					NS <= ST0; Z2 <= '0';
				else
					NS <= WS3; Z2 <= '1';
				end if;
			when others =>
				Z1 <= '0'; Z2 <= '0'; NS <= ST0;
		end case;
	end process comb_proc;

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