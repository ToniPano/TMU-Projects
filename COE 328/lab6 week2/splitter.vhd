library ieee;
use ieee.std_logic_1164.all;

entity splitter is
	port( s	:IN 	STD_LOGIC_VECTOR(3 downto 0);
			s02:OUT STD_LOGIC_VECTOR(2 downto 0);
			sThree	:OUT std_logic);
end entity;

architecture behaviour of splitter is
begin
	s02<=s(2 downto 0);
	sThree<=s(3);
end behaviour;