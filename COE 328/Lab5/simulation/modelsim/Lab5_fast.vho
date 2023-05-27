-- Copyright (C) 1991-2013 Altera Corporation
-- Your use of Altera Corporation's design tools, logic functions 
-- and other software and tools, and its AMPP partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Altera Program License 
-- Subscription Agreement, Altera MegaCore Function License 
-- Agreement, or other applicable license agreement, including, 
-- without limitation, that your use is for the sole purpose of 
-- programming logic devices manufactured by Altera and sold by 
-- Altera or its authorized distributors.  Please refer to the 
-- applicable agreement for further details.

-- VENDOR "Altera"
-- PROGRAM "Quartus II 32-bit"
-- VERSION "Version 13.0.1 Build 232 06/12/2013 Service Pack 1 SJ Full Version"

-- DATE "11/07/2018 12:33:26"

-- 
-- Device: Altera EP2C35F672C6 Package FBGA672
-- 

-- 
-- This VHDL file should be used for ModelSim-Altera (VHDL) only
-- 

LIBRARY CYCLONEII;
LIBRARY IEEE;
USE CYCLONEII.CYCLONEII_COMPONENTS.ALL;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY 	fsm IS
    PORT (
	clk : IN std_logic;
	data_in : IN std_logic;
	reset : IN std_logic;
	student_id : OUT std_logic_vector(3 DOWNTO 0);
	current_state : OUT std_logic_vector(3 DOWNTO 0)
	);
END fsm;

-- Design Ports Information
-- student_id[0]	=>  Location: PIN_P6,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
-- student_id[1]	=>  Location: PIN_T3,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
-- student_id[2]	=>  Location: PIN_R7,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
-- student_id[3]	=>  Location: PIN_T4,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
-- current_state[0]	=>  Location: PIN_T2,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
-- current_state[1]	=>  Location: PIN_R5,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
-- current_state[2]	=>  Location: PIN_P7,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
-- current_state[3]	=>  Location: PIN_R6,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
-- clk	=>  Location: PIN_N25,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: Default
-- reset	=>  Location: PIN_V2,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: Default
-- data_in	=>  Location: PIN_N26,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: Default


ARCHITECTURE structure OF fsm IS
SIGNAL gnd : std_logic := '0';
SIGNAL vcc : std_logic := '1';
SIGNAL unknown : std_logic := 'X';
SIGNAL devoe : std_logic := '1';
SIGNAL devclrn : std_logic := '1';
SIGNAL devpor : std_logic := '1';
SIGNAL ww_devoe : std_logic;
SIGNAL ww_devclrn : std_logic;
SIGNAL ww_devpor : std_logic;
SIGNAL ww_clk : std_logic;
SIGNAL ww_data_in : std_logic;
SIGNAL ww_reset : std_logic;
SIGNAL ww_student_id : std_logic_vector(3 DOWNTO 0);
SIGNAL ww_current_state : std_logic_vector(3 DOWNTO 0);
SIGNAL \clk~clkctrl_INCLK_bus\ : std_logic_vector(3 DOWNTO 0);
SIGNAL \clk~combout\ : std_logic;
SIGNAL \clk~clkctrl_outclk\ : std_logic;
SIGNAL \yfsm.s2~0_combout\ : std_logic;
SIGNAL \reset~combout\ : std_logic;
SIGNAL \data_in~combout\ : std_logic;
SIGNAL \yfsm.s2~regout\ : std_logic;
SIGNAL \yfsm.s4~feeder_combout\ : std_logic;
SIGNAL \yfsm.s4~regout\ : std_logic;
SIGNAL \yfsm.s1~feeder_combout\ : std_logic;
SIGNAL \yfsm.s1~regout\ : std_logic;
SIGNAL \yfsm.s3~feeder_combout\ : std_logic;
SIGNAL \yfsm.s3~regout\ : std_logic;
SIGNAL \yfsm.s8~feeder_combout\ : std_logic;
SIGNAL \yfsm.s8~regout\ : std_logic;
SIGNAL \yfsm.s6~feeder_combout\ : std_logic;
SIGNAL \yfsm.s6~regout\ : std_logic;
SIGNAL \yfsm.s5~feeder_combout\ : std_logic;
SIGNAL \yfsm.s5~regout\ : std_logic;
SIGNAL \yfsm.s7~feeder_combout\ : std_logic;
SIGNAL \yfsm.s7~regout\ : std_logic;
SIGNAL \yfsm.s0~0_combout\ : std_logic;
SIGNAL \yfsm.s0~regout\ : std_logic;
SIGNAL \WideOr2~0_combout\ : std_logic;
SIGNAL \WideOr1~0_combout\ : std_logic;
SIGNAL \WideOr0~combout\ : std_logic;
SIGNAL \WideOr5~0_combout\ : std_logic;
SIGNAL \WideOr4~0_combout\ : std_logic;
SIGNAL \WideOr3~combout\ : std_logic;
SIGNAL \ALT_INV_WideOr1~0_combout\ : std_logic;

BEGIN

ww_clk <= clk;
ww_data_in <= data_in;
ww_reset <= reset;
student_id <= ww_student_id;
current_state <= ww_current_state;
ww_devoe <= devoe;
ww_devclrn <= devclrn;
ww_devpor <= devpor;

\clk~clkctrl_INCLK_bus\ <= (gnd & gnd & gnd & \clk~combout\);
\ALT_INV_WideOr1~0_combout\ <= NOT \WideOr1~0_combout\;

-- Location: PIN_N25,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: Default
\clk~I\ : cycloneii_io
-- pragma translate_off
GENERIC MAP (
	input_async_reset => "none",
	input_power_up => "low",
	input_register_mode => "none",
	input_sync_reset => "none",
	oe_async_reset => "none",
	oe_power_up => "low",
	oe_register_mode => "none",
	oe_sync_reset => "none",
	operation_mode => "input",
	output_async_reset => "none",
	output_power_up => "low",
	output_register_mode => "none",
	output_sync_reset => "none")
-- pragma translate_on
PORT MAP (
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	devoe => ww_devoe,
	oe => GND,
	padio => ww_clk,
	combout => \clk~combout\);

-- Location: CLKCTRL_G6
\clk~clkctrl\ : cycloneii_clkctrl
-- pragma translate_off
GENERIC MAP (
	clock_type => "global clock",
	ena_register_mode => "none")
-- pragma translate_on
PORT MAP (
	inclk => \clk~clkctrl_INCLK_bus\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	outclk => \clk~clkctrl_outclk\);

-- Location: LCCOMB_X1_Y15_N10
\yfsm.s2~0\ : cycloneii_lcell_comb
-- Equation(s):
-- \yfsm.s2~0_combout\ = !\yfsm.s0~regout\

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000011111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datad => \yfsm.s0~regout\,
	combout => \yfsm.s2~0_combout\);

-- Location: PIN_V2,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: Default
\reset~I\ : cycloneii_io
-- pragma translate_off
GENERIC MAP (
	input_async_reset => "none",
	input_power_up => "low",
	input_register_mode => "none",
	input_sync_reset => "none",
	oe_async_reset => "none",
	oe_power_up => "low",
	oe_register_mode => "none",
	oe_sync_reset => "none",
	operation_mode => "input",
	output_async_reset => "none",
	output_power_up => "low",
	output_register_mode => "none",
	output_sync_reset => "none")
-- pragma translate_on
PORT MAP (
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	devoe => ww_devoe,
	oe => GND,
	padio => ww_reset,
	combout => \reset~combout\);

-- Location: PIN_N26,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: Default
\data_in~I\ : cycloneii_io
-- pragma translate_off
GENERIC MAP (
	input_async_reset => "none",
	input_power_up => "low",
	input_register_mode => "none",
	input_sync_reset => "none",
	oe_async_reset => "none",
	oe_power_up => "low",
	oe_register_mode => "none",
	oe_sync_reset => "none",
	operation_mode => "input",
	output_async_reset => "none",
	output_power_up => "low",
	output_register_mode => "none",
	output_sync_reset => "none")
-- pragma translate_on
PORT MAP (
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	devoe => ww_devoe,
	oe => GND,
	padio => ww_data_in,
	combout => \data_in~combout\);

-- Location: LCFF_X1_Y15_N11
\yfsm.s2\ : cycloneii_lcell_ff
PORT MAP (
	clk => \clk~clkctrl_outclk\,
	datain => \yfsm.s2~0_combout\,
	aclr => \reset~combout\,
	ena => \data_in~combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	regout => \yfsm.s2~regout\);

-- Location: LCCOMB_X1_Y15_N30
\yfsm.s4~feeder\ : cycloneii_lcell_comb
-- Equation(s):
-- \yfsm.s4~feeder_combout\ = \yfsm.s2~regout\

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111100000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datad => \yfsm.s2~regout\,
	combout => \yfsm.s4~feeder_combout\);

-- Location: LCFF_X1_Y15_N31
\yfsm.s4\ : cycloneii_lcell_ff
PORT MAP (
	clk => \clk~clkctrl_outclk\,
	datain => \yfsm.s4~feeder_combout\,
	aclr => \reset~combout\,
	ena => \data_in~combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	regout => \yfsm.s4~regout\);

-- Location: LCCOMB_X1_Y15_N22
\yfsm.s1~feeder\ : cycloneii_lcell_comb
-- Equation(s):
-- \yfsm.s1~feeder_combout\ = \yfsm.s4~regout\

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111100000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datad => \yfsm.s4~regout\,
	combout => \yfsm.s1~feeder_combout\);

-- Location: LCFF_X1_Y15_N23
\yfsm.s1\ : cycloneii_lcell_ff
PORT MAP (
	clk => \clk~clkctrl_outclk\,
	datain => \yfsm.s1~feeder_combout\,
	aclr => \reset~combout\,
	ena => \data_in~combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	regout => \yfsm.s1~regout\);

-- Location: LCCOMB_X1_Y15_N12
\yfsm.s3~feeder\ : cycloneii_lcell_comb
-- Equation(s):
-- \yfsm.s3~feeder_combout\ = \yfsm.s1~regout\

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111100000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datad => \yfsm.s1~regout\,
	combout => \yfsm.s3~feeder_combout\);

-- Location: LCFF_X1_Y15_N13
\yfsm.s3\ : cycloneii_lcell_ff
PORT MAP (
	clk => \clk~clkctrl_outclk\,
	datain => \yfsm.s3~feeder_combout\,
	aclr => \reset~combout\,
	ena => \data_in~combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	regout => \yfsm.s3~regout\);

-- Location: LCCOMB_X1_Y15_N28
\yfsm.s8~feeder\ : cycloneii_lcell_comb
-- Equation(s):
-- \yfsm.s8~feeder_combout\ = \yfsm.s3~regout\

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111100000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datad => \yfsm.s3~regout\,
	combout => \yfsm.s8~feeder_combout\);

-- Location: LCFF_X1_Y15_N29
\yfsm.s8\ : cycloneii_lcell_ff
PORT MAP (
	clk => \clk~clkctrl_outclk\,
	datain => \yfsm.s8~feeder_combout\,
	aclr => \reset~combout\,
	ena => \data_in~combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	regout => \yfsm.s8~regout\);

-- Location: LCCOMB_X1_Y15_N0
\yfsm.s6~feeder\ : cycloneii_lcell_comb
-- Equation(s):
-- \yfsm.s6~feeder_combout\ = \yfsm.s8~regout\

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111100000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datad => \yfsm.s8~regout\,
	combout => \yfsm.s6~feeder_combout\);

-- Location: LCFF_X1_Y15_N1
\yfsm.s6\ : cycloneii_lcell_ff
PORT MAP (
	clk => \clk~clkctrl_outclk\,
	datain => \yfsm.s6~feeder_combout\,
	aclr => \reset~combout\,
	ena => \data_in~combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	regout => \yfsm.s6~regout\);

-- Location: LCCOMB_X1_Y15_N18
\yfsm.s5~feeder\ : cycloneii_lcell_comb
-- Equation(s):
-- \yfsm.s5~feeder_combout\ = \yfsm.s6~regout\

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111100000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datad => \yfsm.s6~regout\,
	combout => \yfsm.s5~feeder_combout\);

-- Location: LCFF_X1_Y15_N19
\yfsm.s5\ : cycloneii_lcell_ff
PORT MAP (
	clk => \clk~clkctrl_outclk\,
	datain => \yfsm.s5~feeder_combout\,
	aclr => \reset~combout\,
	ena => \data_in~combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	regout => \yfsm.s5~regout\);

-- Location: LCCOMB_X1_Y15_N6
\yfsm.s7~feeder\ : cycloneii_lcell_comb
-- Equation(s):
-- \yfsm.s7~feeder_combout\ = \yfsm.s5~regout\

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111100000000",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datad => \yfsm.s5~regout\,
	combout => \yfsm.s7~feeder_combout\);

-- Location: LCFF_X1_Y15_N7
\yfsm.s7\ : cycloneii_lcell_ff
PORT MAP (
	clk => \clk~clkctrl_outclk\,
	datain => \yfsm.s7~feeder_combout\,
	aclr => \reset~combout\,
	ena => \data_in~combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	regout => \yfsm.s7~regout\);

-- Location: LCCOMB_X1_Y15_N26
\yfsm.s0~0\ : cycloneii_lcell_comb
-- Equation(s):
-- \yfsm.s0~0_combout\ = !\yfsm.s7~regout\

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000011111111",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	datad => \yfsm.s7~regout\,
	combout => \yfsm.s0~0_combout\);

-- Location: LCFF_X1_Y15_N27
\yfsm.s0\ : cycloneii_lcell_ff
PORT MAP (
	clk => \clk~clkctrl_outclk\,
	datain => \yfsm.s0~0_combout\,
	aclr => \reset~combout\,
	ena => \data_in~combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	regout => \yfsm.s0~regout\);

-- Location: LCCOMB_X1_Y15_N24
\WideOr2~0\ : cycloneii_lcell_comb
-- Equation(s):
-- \WideOr2~0_combout\ = ((\yfsm.s8~regout\) # ((\yfsm.s7~regout\) # (\yfsm.s3~regout\))) # (!\yfsm.s0~regout\)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111111111101",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \yfsm.s0~regout\,
	datab => \yfsm.s8~regout\,
	datac => \yfsm.s7~regout\,
	datad => \yfsm.s3~regout\,
	combout => \WideOr2~0_combout\);

-- Location: LCCOMB_X1_Y15_N14
\WideOr1~0\ : cycloneii_lcell_comb
-- Equation(s):
-- \WideOr1~0_combout\ = (!\yfsm.s6~regout\ & (!\yfsm.s5~regout\ & !\yfsm.s7~regout\))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "0000000100000001",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \yfsm.s6~regout\,
	datab => \yfsm.s5~regout\,
	datac => \yfsm.s7~regout\,
	combout => \WideOr1~0_combout\);

-- Location: LCCOMB_X1_Y15_N16
WideOr0 : cycloneii_lcell_comb
-- Equation(s):
-- \WideOr0~combout\ = ((\yfsm.s5~regout\) # (\yfsm.s6~regout\)) # (!\yfsm.s0~regout\)

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111110111111101",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \yfsm.s0~regout\,
	datab => \yfsm.s5~regout\,
	datac => \yfsm.s6~regout\,
	combout => \WideOr0~combout\);

-- Location: LCCOMB_X1_Y15_N20
\WideOr5~0\ : cycloneii_lcell_comb
-- Equation(s):
-- \WideOr5~0_combout\ = (\yfsm.s3~regout\) # ((\yfsm.s5~regout\) # ((\yfsm.s7~regout\) # (\yfsm.s1~regout\)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111111111110",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \yfsm.s3~regout\,
	datab => \yfsm.s5~regout\,
	datac => \yfsm.s7~regout\,
	datad => \yfsm.s1~regout\,
	combout => \WideOr5~0_combout\);

-- Location: LCCOMB_X1_Y15_N4
\WideOr4~0\ : cycloneii_lcell_comb
-- Equation(s):
-- \WideOr4~0_combout\ = (\yfsm.s6~regout\) # ((\yfsm.s7~regout\) # ((\yfsm.s2~regout\) # (\yfsm.s3~regout\)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111111111110",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \yfsm.s6~regout\,
	datab => \yfsm.s7~regout\,
	datac => \yfsm.s2~regout\,
	datad => \yfsm.s3~regout\,
	combout => \WideOr4~0_combout\);

-- Location: LCCOMB_X1_Y15_N8
WideOr3 : cycloneii_lcell_comb
-- Equation(s):
-- \WideOr3~combout\ = (\yfsm.s6~regout\) # ((\yfsm.s5~regout\) # ((\yfsm.s7~regout\) # (\yfsm.s4~regout\)))

-- pragma translate_off
GENERIC MAP (
	lut_mask => "1111111111111110",
	sum_lutc_input => "datac")
-- pragma translate_on
PORT MAP (
	dataa => \yfsm.s6~regout\,
	datab => \yfsm.s5~regout\,
	datac => \yfsm.s7~regout\,
	datad => \yfsm.s4~regout\,
	combout => \WideOr3~combout\);

-- Location: PIN_P6,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
\student_id[0]~I\ : cycloneii_io
-- pragma translate_off
GENERIC MAP (
	input_async_reset => "none",
	input_power_up => "low",
	input_register_mode => "none",
	input_sync_reset => "none",
	oe_async_reset => "none",
	oe_power_up => "low",
	oe_register_mode => "none",
	oe_sync_reset => "none",
	operation_mode => "output",
	output_async_reset => "none",
	output_power_up => "low",
	output_register_mode => "none",
	output_sync_reset => "none")
-- pragma translate_on
PORT MAP (
	datain => \WideOr2~0_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	devoe => ww_devoe,
	oe => VCC,
	padio => ww_student_id(0));

-- Location: PIN_T3,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
\student_id[1]~I\ : cycloneii_io
-- pragma translate_off
GENERIC MAP (
	input_async_reset => "none",
	input_power_up => "low",
	input_register_mode => "none",
	input_sync_reset => "none",
	oe_async_reset => "none",
	oe_power_up => "low",
	oe_register_mode => "none",
	oe_sync_reset => "none",
	operation_mode => "output",
	output_async_reset => "none",
	output_power_up => "low",
	output_register_mode => "none",
	output_sync_reset => "none")
-- pragma translate_on
PORT MAP (
	datain => \ALT_INV_WideOr1~0_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	devoe => ww_devoe,
	oe => VCC,
	padio => ww_student_id(1));

-- Location: PIN_R7,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
\student_id[2]~I\ : cycloneii_io
-- pragma translate_off
GENERIC MAP (
	input_async_reset => "none",
	input_power_up => "low",
	input_register_mode => "none",
	input_sync_reset => "none",
	oe_async_reset => "none",
	oe_power_up => "low",
	oe_register_mode => "none",
	oe_sync_reset => "none",
	operation_mode => "output",
	output_async_reset => "none",
	output_power_up => "low",
	output_register_mode => "none",
	output_sync_reset => "none")
-- pragma translate_on
PORT MAP (
	datain => \WideOr0~combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	devoe => ww_devoe,
	oe => VCC,
	padio => ww_student_id(2));

-- Location: PIN_T4,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
\student_id[3]~I\ : cycloneii_io
-- pragma translate_off
GENERIC MAP (
	input_async_reset => "none",
	input_power_up => "low",
	input_register_mode => "none",
	input_sync_reset => "none",
	oe_async_reset => "none",
	oe_power_up => "low",
	oe_register_mode => "none",
	oe_sync_reset => "none",
	operation_mode => "output",
	output_async_reset => "none",
	output_power_up => "low",
	output_register_mode => "none",
	output_sync_reset => "none")
-- pragma translate_on
PORT MAP (
	datain => \yfsm.s1~regout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	devoe => ww_devoe,
	oe => VCC,
	padio => ww_student_id(3));

-- Location: PIN_T2,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
\current_state[0]~I\ : cycloneii_io
-- pragma translate_off
GENERIC MAP (
	input_async_reset => "none",
	input_power_up => "low",
	input_register_mode => "none",
	input_sync_reset => "none",
	oe_async_reset => "none",
	oe_power_up => "low",
	oe_register_mode => "none",
	oe_sync_reset => "none",
	operation_mode => "output",
	output_async_reset => "none",
	output_power_up => "low",
	output_register_mode => "none",
	output_sync_reset => "none")
-- pragma translate_on
PORT MAP (
	datain => \WideOr5~0_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	devoe => ww_devoe,
	oe => VCC,
	padio => ww_current_state(0));

-- Location: PIN_R5,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
\current_state[1]~I\ : cycloneii_io
-- pragma translate_off
GENERIC MAP (
	input_async_reset => "none",
	input_power_up => "low",
	input_register_mode => "none",
	input_sync_reset => "none",
	oe_async_reset => "none",
	oe_power_up => "low",
	oe_register_mode => "none",
	oe_sync_reset => "none",
	operation_mode => "output",
	output_async_reset => "none",
	output_power_up => "low",
	output_register_mode => "none",
	output_sync_reset => "none")
-- pragma translate_on
PORT MAP (
	datain => \WideOr4~0_combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	devoe => ww_devoe,
	oe => VCC,
	padio => ww_current_state(1));

-- Location: PIN_P7,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
\current_state[2]~I\ : cycloneii_io
-- pragma translate_off
GENERIC MAP (
	input_async_reset => "none",
	input_power_up => "low",
	input_register_mode => "none",
	input_sync_reset => "none",
	oe_async_reset => "none",
	oe_power_up => "low",
	oe_register_mode => "none",
	oe_sync_reset => "none",
	operation_mode => "output",
	output_async_reset => "none",
	output_power_up => "low",
	output_register_mode => "none",
	output_sync_reset => "none")
-- pragma translate_on
PORT MAP (
	datain => \WideOr3~combout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	devoe => ww_devoe,
	oe => VCC,
	padio => ww_current_state(2));

-- Location: PIN_R6,	 I/O Standard: 3.3-V LVTTL,	 Current Strength: 24mA
\current_state[3]~I\ : cycloneii_io
-- pragma translate_off
GENERIC MAP (
	input_async_reset => "none",
	input_power_up => "low",
	input_register_mode => "none",
	input_sync_reset => "none",
	oe_async_reset => "none",
	oe_power_up => "low",
	oe_register_mode => "none",
	oe_sync_reset => "none",
	operation_mode => "output",
	output_async_reset => "none",
	output_power_up => "low",
	output_register_mode => "none",
	output_sync_reset => "none")
-- pragma translate_on
PORT MAP (
	datain => \yfsm.s8~regout\,
	devclrn => ww_devclrn,
	devpor => ww_devpor,
	devoe => ww_devoe,
	oe => VCC,
	padio => ww_current_state(3));
END structure;


