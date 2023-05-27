library ieee;
use ieee.std_logic_1164.all;

entity decod is
	port( s02	:in std_logic_vector(2 downto 0);
			En	:in std_logic;
			y	:out std_logic_vector(7 downto 0));
end decod;

architecture behaviour of decod is
	signal Enw: std_logic_vector(3 downto 0); 
begin
		Enw <= En & s02;
		with Enw select
			y <=	"00000001" when "1000",
					"00000010" when "1001",
					"00000100" when "1010",
					"00001000" when "1011",
					"00010000" when "1100",
					"00100000" when "1101",
					"01000000" when "1110",
					"10000000" when "1111",
					"00000000" when others;
end behaviour;