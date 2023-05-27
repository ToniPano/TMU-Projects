library ieee;
use ieee.std_logic_1164.all;

entity fsm is
	port
	(
		clk	  : in std_logic;
		data_in : in std_logic;
		reset   : in std_logic;
		student_id  : out std_logic_vector( 3 downto 0);
		current_state : out std_logic_vector(3 downto 0)
	);
	end entity;
	
	architecture machine of fsm is
		type state_type is (s0,s1,s2,s3,s4,s5,s6,s7,s8);
		signal yfsm : state_type;
	begin	
		process(clk,reset,data_in)
		begin
			if reset = '1' then
				yfsm<=s0;
			elsif(clk'EVENT AND clk ='1') then
				if(data_in = '1') then
					case yfsm is
					
						when s0=> yfsm <= s2;
								
						when s1=> yfsm <= s3;
						
						when s2=> yfsm <= s4;
						
						when s3=> yfsm <= s8;
						
						when s4=> yfsm <= s1;
						
						when s5=> yfsm <= s7;
						
						when s6=> yfsm <= s5;
						
						when s7=> yfsm <= s0;
						
						when s8=> yfsm <= s6;
						
					end case;
				end if;
			end if;
		end process;
			
			process(yfsm)
			begin	
				case yfsm is
					when s0=>
						student_id <= "0101";
						current_state <= "0000";
						
					when s2=>
						student_id <= "0000";
						current_state <= "0010";
					
					when s4=>
						student_id <= "0000";
						current_state <= "0100";
					
					when s1=>
						student_id <= "1000";
						current_state <= "0001";
					when s3=>
						student_id <= "0001";
						current_state <= "0011";
						
					when s8=>
						student_id <= "0001";
						current_state <= "1000";
					
					when s6=>
						student_id <= "0110";
						current_state <= "0110";
						
					when s5=>
						student_id <= "0110";
						current_state <= "0101";
					
					when s7=>
						student_id <= "0011";
						current_state <= "0111";
					
				end case;
			end process;
			
			end machine;
					