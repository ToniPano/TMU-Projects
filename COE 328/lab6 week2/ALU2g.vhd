library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity ALU2g is
	port(clock			:in	std_logic;
			A,B			:in	unsigned(7 downto 0);
			student_id	:in 	unsigned(3 downto 0);
			OP				:in 	unsigned (15 downto 0);
			neg			:out	std_logic;
			R1				:out 	unsigned(3 downto 0);
			R2				:out 	unsigned(3 downto 0));
end ALU2g;

architecture calculation of ALU2g is
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
			when "0000000000000001" =>--invert bit significance of a
				for i in 0 to 7 loop
					result(i)<=reg1(7-i);
				end loop;
				
				neg<='0';
				
			when "0000000000000010" =>--shift A to left by 4 bits, fill with 1
				result<=reg1(3 downto 0) & "1111";
				--result<=shift_left(reg1,4);
				neg<='0';
				
			when "0000000000000100" =>--invert the 4 msb of B
				result<=not(reg2(7 downto 4)) & reg2(3 downto 0);
				neg<='0';
				
			when "0000000000001000" =>--minimum of A and B
				if(reg1<reg2)then
					result<=reg1;
				else
					result<=reg2;
				end if;
				neg<='0';
				
			when "0000000000010000" =>--A+B+4
				result <=reg1 + reg2 + "00000100";
				neg<='0';
				
			when "0000000000100000" =>--A+3
				result <=reg1 + "00000011";
				neg<='0';
				
			when "0000000001000000" =>--replace even bits of A with even bits of B
				result <=reg1;
				
				for i in 0 to 3 loop
					result(i*2)<=reg2(i*2);
				end loop;
				
				neg<='0';
				
			when "0000000010000000" =>--boolean A xnor B
				result <=reg1 XNOR reg2;
				neg<='0';
				
			when "0000000100000000" =>--rotate B to the right by 3 bits
				result <=reg2 ror 3;
				neg<='0';
				
			when others =>--do nothing
			end case;
		end if;
	end process;
	
	r1<=result(3 downto 0);
	r2<=result(7 downto 4);
end calculation;