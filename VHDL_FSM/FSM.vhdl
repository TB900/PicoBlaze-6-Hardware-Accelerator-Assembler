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
			Y : out std_logic_vector(9 downto 0)
		);
end my_fsm;

architecture fsm_arch of my_fsm is
	type state_type is (
	signal PS, NS : state_type;
begin
	sync_proc : process(CLK, NS, RESET)
	begin
		if (RESET = '1') then PS <= ST0;
		elsif (rising_edge(CLK)) then
			case PS is
				when others =>
					 PS <= ST0;
			end case;
		end if;
	end process sync_proc;

	with PS select
		Y <= "0000000000" when others;
end fsm_arch;