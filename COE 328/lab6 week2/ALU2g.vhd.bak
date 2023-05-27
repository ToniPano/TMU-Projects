library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity ALU is
	port(clock			:in	std_logic;
			A,B			:in	unsigned(7 downto 0);
			student_id	:in 	unsigned(3 downto 0);
			OP				:in 	unsigned (15 downto 0);
			neg			:out	std_logic;
			R1				:out 	unsigned(3 downto 0);
			R2				:out 	unsigned(3 downto 0));
end ALU;

architecture calculation of ALU is
signal reg1,reg2,result: unsigned(7 downto 0):=(others=>'0');
signal reg4 : unsigned(8 downto 0);
signal minus: std_logic;

begin
	reg1 <= A;
	reg2 <= B;
	
	process(clock,OP)
	begin
	
		if(rising_edge(clock))then
			case OP is
			when "0000000000000001" =>--add a and b
				result<=reg1+reg2;
				neg<='0';
				
			when "0000000000000010" =>--subtract b from a
				reg4 <= ('0' & reg1)- reg2;
				
				if(A<B) then
					neg<='1';
					result<=not(reg4(7 downto 0))+1;
				else
					neg<='0';
					result<=reg4(7 downto 0);
				end if;
				
			when "0000000000000100" =>--invert a
				result<=not(reg1);
				neg<='0';
				
			when "0000000000001000" =>--boolean a nand b
				result <=reg1 NAND reg2;
				neg<='0';
				
			when "0000000000010000" =>--boolean a nor b
				result <=reg1 NOR reg2;
				neg<='0';
				
			when "0000000000100000" =>--boolean a and b
				result <=reg1 AND reg2;
				neg<='0';
				
			when "0000000001000000" =>--boolean a xor b
				result <=reg1 XOR reg2;
				neg<='0';
				
			when "0000000010000000" =>--boolean a or b
				result <=reg1 OR reg2;
				neg<='0';
				
			when "0000000100000000" =>--boolean a xnor b
				result <=reg1 XNOR reg2;
				neg<='0';
				
			when others =>--do nothing
			end case;
		end if;
	end process;
	
	r1<=result(3 downto 0);
	r2<=result(7 downto 4);
end calculation;