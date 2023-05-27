library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity ALU2h is
	port(clock			:in	std_logic;
			A,B			:in	unsigned(7 downto 0);
			student_id	:in 	unsigned(3 downto 0);
			OP				:in 	unsigned (15 downto 0);
			neg			:out	std_logic;
			R1				:out 	unsigned(3 downto 0);
			R2				:out 	unsigned(3 downto 0));
end ALU2h;

architecture calculation of ALU2h is
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
			when "0000000000000001" =>--rotate a to the right by 4
				result<= reg1 ror 4;
				neg<='0';
				
			when "0000000000000010" =>--a xor b
				result <=reg1 XOR reg2;
				neg<='0';
				
			when "0000000000000100" =>--invert the bit significance of B
				for i in 0 to 7 loop
					result(i)<=reg2(7-i);
				end loop;
				
				neg<='0';
				
			when "0000000000001000" =>--A+B-2
				
				if((reg1 + reg2)<"00000010") then
					neg<='1';
					result<=not(reg1 + reg2 + not("00000010") + 1)+1;
				else
					neg<='0';
					result<=reg1+reg2 + not("00000010") + 1;
				end if;
				
			when "0000000000010000" =>--rotate B to the left by 2
				result <=reg2 rol 2;
				neg<='0';
				
			when "0000000000100000" =>--invert the even bits of B
				result <=reg2;
				
				for i in 0 to 3 loop
					result(i*2)<=not(reg2(i*2));
				end loop;
				
				neg<='0';
				
			when "0000000001000000" =>--swap the 4 lsb of B with the 4 lsb of A
				result <=reg2(7 downto 4) & reg1(3 downto 0);
				neg<='0';
				
			when "0000000010000000" =>--shift B to the right by 2, fill with 0
				result <=reg2 srl 2;
				neg<='0';
				
			when "0000000100000000" =>--invert lower 4 bits of A
				result <=reg1(7 downto 4) & not(reg1(3 downto 0));
				neg<='0';
				
			when others =>--do nothing
			end case;
		end if;
	end process;
	
	r1<=result(3 downto 0);
	r2<=result(7 downto 4);
end calculation;