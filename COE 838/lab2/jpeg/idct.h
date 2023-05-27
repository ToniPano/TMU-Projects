#include "systemc.h"
#include <math.h>

#ifndef IDCT_H
#define IDCT_H

struct idct : sc_module{
	sc_in<double> in64[8][8]; //the dc transformed 8x8 block
	sc_in<double> fcosine[8][8]; //cosine table input
	sc_out<FILE *> sc_output; //output file pointer port
	sc_in<bool> clk; //clock signal
	
	char output_data[8][8]; //the data to write to the output file
	
	void write_data( void ); //write the transformed 8x8 block
	void calculate_idct(void ); //perform inverse discrete cosine transform
	
	//define idct as constructor
	SC_CTOR( idct ){
		SC_METHOD( calculate_idct );
		dont_initialize();
		sensitive << clk.pos();
		
		SC_METHOD( write_data );
		dont_initialize();
		sensitive << clk.neg();
	}
};

#endif
