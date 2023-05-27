LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;

ENTITY ssegYN IS
	PORT(bcd : IN	STD_LOGIC_VECTOR(1 DOWNTO 0);
		  leds: OUT	STD_LOGIC_VECTOR(1 TO 7));
END ssegYN;

ARCHITECTURE Behavior OF ssegYN IS
BEGIN
	PROCESS(bcd)
	BEGIN
		CASE bcd IS				--   abcdefg  --#,[lights that are on]
			WHEN "01"=> leds <=not("0111011");--y, everything but a and e
			WHEN "10"=> leds <=not("0010101");--n, only c, e, g
			
			WHEN OTHERS	=> leds <=not("0000000");--default case (everything is off)
		END CASE;
	END PROCESS;
END Behavior;