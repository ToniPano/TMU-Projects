library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity ALU3g is
	port(clock			:in	std_logic;
			A,B			:in	unsigned(7 downto 0);
			student_id	:in 	unsigned(3 downto 0);
			OP				:in 	unsigned (15 downto 0);
			yn				:out 	std_logic_vector(1 downto 0));
end ALU3g;

architecture calculation of ALU3g is
begin
	process(clock,OP)
	begin
		if(rising_edge(clock))then
			if (OP < "0000001000000000") then
				if ((student_id = A(7 downto 4)) OR (student_id = A(3 downto 0))) then
					yn <= "01";
				else
					yn <= "10";
				end if;
			else yn <="00";
			end if;
		end if;
	end process;
end calculation;