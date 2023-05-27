/*
 * sw.c
 *
 *  Created on: 2022-02-10
 * 	Adapted from the led.c file for COE 838 Lab 3
 */

#include "sw.h"
#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"

extern volatile unsigned long* h2p_lw_sw_addr;

uint32_t readSwitches(void){
	uint32_t sw = alt_read_word(h2p_lw_sw_addr);

	//bits 0 to 9 are switch states, bits 10 to 15 are not switches and should be set to 0
	uint16_t bitMask = 0x000003ff;
	sw = sw & bitMask;

	return sw;
}
