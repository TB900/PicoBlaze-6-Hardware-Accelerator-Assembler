library ieee;
use ieee.std_logic_1164.all;

entity my_fsm is
	 port ( X, CLK, RESET : in std_logic;
			Y : out std_logic_vector(1 downto 0);
			Z1, Z2 : out std_logic);
end my_fsm;

architecture fsm_arch of my_fsm is
	type state_type is (ST0, ST1, ST2, ST3, ST4, ST5, ST6, ST7, ST8, ST9, ST10, ST11, ST12, ST13, ST14, ST15, ST16, ST17, ST18, ST19, ST20, ST21, ST22, ST23, ST24, ST25, ST26, ST27, ST28, ST29, ST30, ST31, ST32, ST33, ST34, ST35, ST36, ST37, ST38, ST39, ST40, ST41, ST42, ST43, ST44, ST45, ST46, ST47, ST48, ST49, ST50, ST51, ST52, ST53, ST54, ST55, ST56, ST57, ST58, ST59, ST60, ST61, ST62, ST63, ST64, ST65, ST66, ST67, ST68, ST69, ST70, ST71, ST72, ST73, ST74, ST75, ST76, ST77, ST78, ST79, ST80, ST81, ST82, ST83, ST84, ST85, ST86, ST87, ST88, ST89, ST90, ST91, ST92, ST93, ST94, ST95, ST96, ST97, ST98, ST99);
	signal PS, NS : state_type;
begin
	sync_proc : process(CLK, NS, RESET)
	begin
		if (RESET = '1') then PS <= ST0
		elsif (rising_edge(CLK)) then PS <= NS;
		end if;
	end process sync_proc;

	comb_proc : process(PS, X)
	begin
		Z1 <= '0'; Z2 <= '0';
		case PS is
			when ST0 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST1; Z2 <= '0';
				else
					NS <= ST0; Z2 <= '1';
				end if;
			when ST1 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST2; Z2 <= '0';
				else
					NS <= ST1; Z2 <= '1';
				end if;
			when ST2 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST3; Z2 <= '0';
				else
					NS <= ST2; Z2 <= '1';
				end if;
			when ST3 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST4; Z2 <= '0';
				else
					NS <= ST3; Z2 <= '1';
				end if;
			when ST4 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST5; Z2 <= '0';
				else
					NS <= ST4; Z2 <= '1';
				end if;
			when ST5 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST6; Z2 <= '0';
				else
					NS <= ST5; Z2 <= '1';
				end if;
			when ST6 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST7; Z2 <= '0';
				else
					NS <= ST6; Z2 <= '1';
				end if;
			when ST7 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST8; Z2 <= '0';
				else
					NS <= ST7; Z2 <= '1';
				end if;
			when ST8 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST9; Z2 <= '0';
				else
					NS <= ST8; Z2 <= '1';
				end if;
			when ST9 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST10; Z2 <= '0';
				else
					NS <= ST9; Z2 <= '1';
				end if;
			when ST10 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST11; Z2 <= '0';
				else
					NS <= ST10; Z2 <= '1';
				end if;
			when ST11 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST12; Z2 <= '0';
				else
					NS <= ST11; Z2 <= '1';
				end if;
			when ST12 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST13; Z2 <= '0';
				else
					NS <= ST12; Z2 <= '1';
				end if;
			when ST13 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST14; Z2 <= '0';
				else
					NS <= ST13; Z2 <= '1';
				end if;
			when ST14 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST15; Z2 <= '0';
				else
					NS <= ST14; Z2 <= '1';
				end if;
			when ST15 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST16; Z2 <= '0';
				else
					NS <= ST15; Z2 <= '1';
				end if;
			when ST16 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST17; Z2 <= '0';
				else
					NS <= ST16; Z2 <= '1';
				end if;
			when ST17 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST18; Z2 <= '0';
				else
					NS <= ST17; Z2 <= '1';
				end if;
			when ST18 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST19; Z2 <= '0';
				else
					NS <= ST18; Z2 <= '1';
				end if;
			when ST19 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST20; Z2 <= '0';
				else
					NS <= ST19; Z2 <= '1';
				end if;
			when ST20 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST21; Z2 <= '0';
				else
					NS <= ST20; Z2 <= '1';
				end if;
			when ST21 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST22; Z2 <= '0';
				else
					NS <= ST21; Z2 <= '1';
				end if;
			when ST22 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST23; Z2 <= '0';
				else
					NS <= ST22; Z2 <= '1';
				end if;
			when ST23 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST24; Z2 <= '0';
				else
					NS <= ST23; Z2 <= '1';
				end if;
			when ST24 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST25; Z2 <= '0';
				else
					NS <= ST24; Z2 <= '1';
				end if;
			when ST25 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST26; Z2 <= '0';
				else
					NS <= ST25; Z2 <= '1';
				end if;
			when ST26 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST27; Z2 <= '0';
				else
					NS <= ST26; Z2 <= '1';
				end if;
			when ST27 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST28; Z2 <= '0';
				else
					NS <= ST27; Z2 <= '1';
				end if;
			when ST28 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST29; Z2 <= '0';
				else
					NS <= ST28; Z2 <= '1';
				end if;
			when ST29 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST30; Z2 <= '0';
				else
					NS <= ST29; Z2 <= '1';
				end if;
			when ST30 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST31; Z2 <= '0';
				else
					NS <= ST30; Z2 <= '1';
				end if;
			when ST31 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST32; Z2 <= '0';
				else
					NS <= ST31; Z2 <= '1';
				end if;
			when ST32 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST33; Z2 <= '0';
				else
					NS <= ST32; Z2 <= '1';
				end if;
			when ST33 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST34; Z2 <= '0';
				else
					NS <= ST33; Z2 <= '1';
				end if;
			when ST34 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST35; Z2 <= '0';
				else
					NS <= ST34; Z2 <= '1';
				end if;
			when ST35 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST36; Z2 <= '0';
				else
					NS <= ST35; Z2 <= '1';
				end if;
			when ST36 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST37; Z2 <= '0';
				else
					NS <= ST36; Z2 <= '1';
				end if;
			when ST37 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST38; Z2 <= '0';
				else
					NS <= ST37; Z2 <= '1';
				end if;
			when ST38 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST39; Z2 <= '0';
				else
					NS <= ST38; Z2 <= '1';
				end if;
			when ST39 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST40; Z2 <= '0';
				else
					NS <= ST39; Z2 <= '1';
				end if;
			when ST40 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST41; Z2 <= '0';
				else
					NS <= ST40; Z2 <= '1';
				end if;
			when ST41 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST42; Z2 <= '0';
				else
					NS <= ST41; Z2 <= '1';
				end if;
			when ST42 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST43; Z2 <= '0';
				else
					NS <= ST42; Z2 <= '1';
				end if;
			when ST43 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST44; Z2 <= '0';
				else
					NS <= ST43; Z2 <= '1';
				end if;
			when ST44 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST45; Z2 <= '0';
				else
					NS <= ST44; Z2 <= '1';
				end if;
			when ST45 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST46; Z2 <= '0';
				else
					NS <= ST45; Z2 <= '1';
				end if;
			when ST46 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST47; Z2 <= '0';
				else
					NS <= ST46; Z2 <= '1';
				end if;
			when ST47 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST48; Z2 <= '0';
				else
					NS <= ST47; Z2 <= '1';
				end if;
			when ST48 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST49; Z2 <= '0';
				else
					NS <= ST48; Z2 <= '1';
				end if;
			when ST49 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST50; Z2 <= '0';
				else
					NS <= ST49; Z2 <= '1';
				end if;
			when ST50 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST51; Z2 <= '0';
				else
					NS <= ST50; Z2 <= '1';
				end if;
			when ST51 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST52; Z2 <= '0';
				else
					NS <= ST51; Z2 <= '1';
				end if;
			when ST52 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST53; Z2 <= '0';
				else
					NS <= ST52; Z2 <= '1';
				end if;
			when ST53 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST54; Z2 <= '0';
				else
					NS <= ST53; Z2 <= '1';
				end if;
			when ST54 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST55; Z2 <= '0';
				else
					NS <= ST54; Z2 <= '1';
				end if;
			when ST55 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST56; Z2 <= '0';
				else
					NS <= ST55; Z2 <= '1';
				end if;
			when ST56 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST57; Z2 <= '0';
				else
					NS <= ST56; Z2 <= '1';
				end if;
			when ST57 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST58; Z2 <= '0';
				else
					NS <= ST57; Z2 <= '1';
				end if;
			when ST58 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST59; Z2 <= '0';
				else
					NS <= ST58; Z2 <= '1';
				end if;
			when ST59 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST60; Z2 <= '0';
				else
					NS <= ST59; Z2 <= '1';
				end if;
			when ST60 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST61; Z2 <= '0';
				else
					NS <= ST60; Z2 <= '1';
				end if;
			when ST61 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST62; Z2 <= '0';
				else
					NS <= ST61; Z2 <= '1';
				end if;
			when ST62 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST63; Z2 <= '0';
				else
					NS <= ST62; Z2 <= '1';
				end if;
			when ST63 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST64; Z2 <= '0';
				else
					NS <= ST63; Z2 <= '1';
				end if;
			when ST64 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST65; Z2 <= '0';
				else
					NS <= ST64; Z2 <= '1';
				end if;
			when ST65 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST66; Z2 <= '0';
				else
					NS <= ST65; Z2 <= '1';
				end if;
			when ST66 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST67; Z2 <= '0';
				else
					NS <= ST66; Z2 <= '1';
				end if;
			when ST67 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST68; Z2 <= '0';
				else
					NS <= ST67; Z2 <= '1';
				end if;
			when ST68 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST69; Z2 <= '0';
				else
					NS <= ST68; Z2 <= '1';
				end if;
			when ST69 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST70; Z2 <= '0';
				else
					NS <= ST69; Z2 <= '1';
				end if;
			when ST70 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST71; Z2 <= '0';
				else
					NS <= ST70; Z2 <= '1';
				end if;
			when ST71 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST72; Z2 <= '0';
				else
					NS <= ST71; Z2 <= '1';
				end if;
			when ST72 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST73; Z2 <= '0';
				else
					NS <= ST72; Z2 <= '1';
				end if;
			when ST73 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST74; Z2 <= '0';
				else
					NS <= ST73; Z2 <= '1';
				end if;
			when ST74 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST75; Z2 <= '0';
				else
					NS <= ST74; Z2 <= '1';
				end if;
			when ST75 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST76; Z2 <= '0';
				else
					NS <= ST75; Z2 <= '1';
				end if;
			when ST76 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST77; Z2 <= '0';
				else
					NS <= ST76; Z2 <= '1';
				end if;
			when ST77 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST78; Z2 <= '0';
				else
					NS <= ST77; Z2 <= '1';
				end if;
			when ST78 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST79; Z2 <= '0';
				else
					NS <= ST78; Z2 <= '1';
				end if;
			when ST79 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST80; Z2 <= '0';
				else
					NS <= ST79; Z2 <= '1';
				end if;
			when ST80 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST81; Z2 <= '0';
				else
					NS <= ST80; Z2 <= '1';
				end if;
			when ST81 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST82; Z2 <= '0';
				else
					NS <= ST81; Z2 <= '1';
				end if;
			when ST82 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST83; Z2 <= '0';
				else
					NS <= ST82; Z2 <= '1';
				end if;
			when ST83 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST84; Z2 <= '0';
				else
					NS <= ST83; Z2 <= '1';
				end if;
			when ST84 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST85; Z2 <= '0';
				else
					NS <= ST84; Z2 <= '1';
				end if;
			when ST85 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST86; Z2 <= '0';
				else
					NS <= ST85; Z2 <= '1';
				end if;
			when ST86 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST87; Z2 <= '0';
				else
					NS <= ST86; Z2 <= '1';
				end if;
			when ST87 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST88; Z2 <= '0';
				else
					NS <= ST87; Z2 <= '1';
				end if;
			when ST88 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST89; Z2 <= '0';
				else
					NS <= ST88; Z2 <= '1';
				end if;
			when ST89 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST90; Z2 <= '0';
				else
					NS <= ST89; Z2 <= '1';
				end if;
			when ST90 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST91; Z2 <= '0';
				else
					NS <= ST90; Z2 <= '1';
				end if;
			when ST91 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST92; Z2 <= '0';
				else
					NS <= ST91; Z2 <= '1';
				end if;
			when ST92 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST93; Z2 <= '0';
				else
					NS <= ST92; Z2 <= '1';
				end if;
			when ST93 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST94; Z2 <= '0';
				else
					NS <= ST93; Z2 <= '1';
				end if;
			when ST94 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST95; Z2 <= '0';
				else
					NS <= ST94; Z2 <= '1';
				end if;
			when ST95 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST96; Z2 <= '0';
				else
					NS <= ST95; Z2 <= '1';
				end if;
			when ST96 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST97; Z2 <= '0';
				else
					NS <= ST96; Z2 <= '1';
				end if;
			when ST97 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST98; Z2 <= '0';
				else
					NS <= ST97; Z2 <= '1';
				end if;
			when ST98 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST99; Z2 <= '0';
				else
					NS <= ST98; Z2 <= '1';
				end if;
			when ST99 =>
				Z1 <= '0';
				if (X = '0') then
					NS <= ST0; Z2 <= '0';
				else
					NS <= ST99; Z2 <= '1';
				end if;
			when others =>
				Z1 <= '0'; Z2 <= '0'; NS <= ST0;
		end case;
	end process comb_proc;

	with PS select
		Y <= '0' when ST0,
			 '1' when ST1,
			 '2' when ST2,
			 '3' when ST3,
			 '4' when ST4,
			 '5' when ST5,
			 '6' when ST6,
			 '7' when ST7,
			 '8' when ST8,
			 '9' when ST9,
			 '10' when ST10,
			 '11' when ST11,
			 '12' when ST12,
			 '13' when ST13,
			 '14' when ST14,
			 '15' when ST15,
			 '16' when ST16,
			 '17' when ST17,
			 '18' when ST18,
			 '19' when ST19,
			 '20' when ST20,
			 '21' when ST21,
			 '22' when ST22,
			 '23' when ST23,
			 '24' when ST24,
			 '25' when ST25,
			 '26' when ST26,
			 '27' when ST27,
			 '28' when ST28,
			 '29' when ST29,
			 '30' when ST30,
			 '31' when ST31,
			 '32' when ST32,
			 '33' when ST33,
			 '34' when ST34,
			 '35' when ST35,
			 '36' when ST36,
			 '37' when ST37,
			 '38' when ST38,
			 '39' when ST39,
			 '40' when ST40,
			 '41' when ST41,
			 '42' when ST42,
			 '43' when ST43,
			 '44' when ST44,
			 '45' when ST45,
			 '46' when ST46,
			 '47' when ST47,
			 '48' when ST48,
			 '49' when ST49,
			 '50' when ST50,
			 '51' when ST51,
			 '52' when ST52,
			 '53' when ST53,
			 '54' when ST54,
			 '55' when ST55,
			 '56' when ST56,
			 '57' when ST57,
			 '58' when ST58,
			 '59' when ST59,
			 '60' when ST60,
			 '61' when ST61,
			 '62' when ST62,
			 '63' when ST63,
			 '64' when ST64,
			 '65' when ST65,
			 '66' when ST66,
			 '67' when ST67,
			 '68' when ST68,
			 '69' when ST69,
			 '70' when ST70,
			 '71' when ST71,
			 '72' when ST72,
			 '73' when ST73,
			 '74' when ST74,
			 '75' when ST75,
			 '76' when ST76,
			 '77' when ST77,
			 '78' when ST78,
			 '79' when ST79,
			 '80' when ST80,
			 '81' when ST81,
			 '82' when ST82,
			 '83' when ST83,
			 '84' when ST84,
			 '85' when ST85,
			 '86' when ST86,
			 '87' when ST87,
			 '88' when ST88,
			 '89' when ST89,
			 '90' when ST90,
			 '91' when ST91,
			 '92' when ST92,
			 '93' when ST93,
			 '94' when ST94,
			 '95' when ST95,
			 '96' when ST96,
			 '97' when ST97,
			 '98' when ST98,
			 '99' when ST99,
			 '0' when others;
end my_fsm;