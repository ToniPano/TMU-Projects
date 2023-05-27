#include <iostream>
#include "barrel_shifter.h"

sc_int<8> d_shifted;
int bit, shifted_bit;

void barrel_shifter :: behaviour(){
	if(en.read() == 1){	//see if enabled
		if(l_r.read() == 0){ //shift left (assuming MSB on left)
			shifted_bit = shift_amt.read();
		}
		else{ //shift right (assuming MSB on left)
			shifted_bit = 8 - shift_amt.read();			
		}
		
		for(bit = 0; bit < 8; bit++){
			if(shifted_bit == 8){	//mke shifted bit loop back to beginning of data
				shifted_bit = 0;
			}			
			
			d_shifted[shifted_bit] = d_in.read()[bit];
			shifted_bit++;
		}
		
		for(bit = 0; bit < 8; bit++){		
			cout << "\t\td_in[" << bit << "]= " <<  d_in.read()[bit] << ", d_out[" << bit << "]= " << d_shifted[bit] << endl;
		}
		
		d_out.write(d_shifted); //write shifted result to output variable
	}
	else{	//see if disabled
		d_out.write(0);
	}
	
	cout << "\tBS en: " << en.read() << ", l_r: " << l_r.read() << ", shift_amt: " << shift_amt.read() << ", d_in: " << d_in.read() << ", d_out: " << d_shifted << endl;
}
