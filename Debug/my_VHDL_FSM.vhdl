library ieee;
use ieee.std_logic_1164.all;

entity my_fsm is
	 port ( X, CLK, RESET : in std_logic;
			Y : out std_logic_vector(1 downto 0);
			Z1, Z2 : out std_logic);
end my_fsm;

architecture fsm_arch of my_fsm is
	type state_type is (ST0, ST1, ST2, ST3, ST4, ST5, ST6, ST7, ST8, ST9);
	signal PS, NS : state_type;
