library ieee;
use ieee.std_logic_1164.all;

entity combiner is
	port(OPlsb,OPmsb	:IN 	STD_LOGIC_VECTOR(7 downto 0);
			OP				:OUT STD_LOGIC_VECTOR(15 downto 0));
end entity;

architecture behaviour of combiner is
begin
	OP<= OPmsb & OPlsb;
end behaviour;