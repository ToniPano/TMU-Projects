library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

ENTITY div_unit IS
	PORT(
		clk, reset, enable	: IN STD_LOGIC;
		div_denom, div_numer	: IN STD_LOGIC_VECTOR(15 DOWNTO 0);
		div_done						: OUT STD_LOGIC;
		div_quot, div_rem		: OUT STD_LOGIC_VECTOR(15 DOWNTO 0)
	);
END ENTITY div_unit;

ARCHITECTURE Behaviour OF div_unit IS
	COMPONENT div
		PORT(
			aclr		: IN STD_LOGIC ;
			clken		: IN STD_LOGIC ;
			clock		: IN STD_LOGIC ;
			denom		: IN STD_LOGIC_VECTOR (15 DOWNTO 0);
			numer		: IN STD_LOGIC_VECTOR (15 DOWNTO 0);
			quotient		: OUT STD_LOGIC_VECTOR (15 DOWNTO 0);
			remain		: OUT STD_LOGIC_VECTOR (15 DOWNTO 0)
		);
	END COMPONENT;
	
	SIGNAL done0, done1, done2, done3 : STD_LOGIC;
	SIGNAL temp_q, temp_r 		: STD_LOGIC_VECTOR(15 DOWNTO 0);
	
BEGIN
	divider : div
	PORT MAP(
		aclr => reset,
		clken => enable,
		clock => clk,
		denom => div_denom,
		numer => div_numer,
		quotient => temp_q,
		remain => temp_r
	);
	
	PROCESS(clk, reset, enable)
	BEGIN
		IF(reset = '1')THEN
			done0 <= '0';
			done1 <= '0';
			done2 <= '0';
			done3 <= '0';
		
		ELSIF(rising_edge(clk))THEN
			IF(enable = '1')THEN
				done0 <= '1';
			END IF;
			
			--pipeline cycle counting
			done1 <= done0;
			done2 <= done1;
			done3 <= done2;
		
		END IF;
	END PROCESS;
	
	div_quot <= temp_q;
	div_rem <= temp_r;
	div_done <= done3;
	
END Behaviour;