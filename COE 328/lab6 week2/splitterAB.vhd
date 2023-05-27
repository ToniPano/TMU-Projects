library ieee;
use ieee.std_logic_1164.all;

entity splitterAB is
	port( s	:IN 	STD_LOGIC_VECTOR(7 downto 0);
			slsb,smsb:OUT STD_LOGIC_VECTOR(3 downto 0));
end entity;

architecture behaviour of splitterAB is
begin
	smsb<=s(7 downto 4);
	slsb<=s(3 downto 0);
end behaviour;