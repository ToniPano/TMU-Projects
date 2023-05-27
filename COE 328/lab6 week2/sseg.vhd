LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;

ENTITY sseg IS
	PORT(bcd : IN	STD_LOGIC_VECTOR(3 DOWNTO 0);
		  neg : IN STD_LOGIC;
		  leds: OUT	STD_LOGIC_VECTOR(1 TO 7);
		  negOut: OUT	STD_LOGIC_VECTOR(1 TO 7));
END sseg;

ARCHITECTURE Behavior OF sseg IS
BEGIN
	PROCESS(bcd, neg)
	BEGIN
		CASE bcd IS				--   abcdefg  --#,[lights that are on]
			WHEN "0000"	=> leds <=not("1111110");--0, everything but g
			WHEN "0001"	=> leds <=not("0110000");--1, only b, c
			WHEN "0010"	=> leds <=not("1101101");--2, everything but c, f
			WHEN "0011"	=> leds <=not("1111001");--3, everything but e, f
			
			WHEN "0100"	=> leds <=not("0110011");--4, everything but a, d, e
			WHEN "0101"	=> leds <=not("1011011");--5, everything but b, e
			WHEN "0110"	=> leds <=not("1011111");--6, everything but b
			WHEN "0111"	=> leds <=not("1110000");--7, only a, b, c
			
			WHEN "1000"	=> leds <=not("1111111");--8, everything
			WHEN "1001"	=> leds <=not("1111011");--9, everything but e
			WHEN "1010"	=> leds <=not("1110111");--10=A, everything but d
			WHEN "1011"	=> leds <=not("0011111");--11=b, everything but a, b
			
			WHEN "1100"	=> leds <=not("1001110");--12=C, everything but b, c, g
			WHEN "1101"	=> leds <=not("0111101");--13=d, everything but a,f
			WHEN "1110"	=> leds <=not("1001111");--14=E, everything but b, c
			WHEN "1111"	=> leds <=not("1000111");--15=F, everything but b, c, d
			
			WHEN OTHERS	=> leds <=not("0000000");--default case (everything is off)
		END CASE;
		
		IF neg='1' THEN
			negOut <= not("0000001");
		ELSE
			negOut <=not("0000000");
		END IF;
	END PROCESS;
END Behavior;