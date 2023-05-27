#ifndef BARREL_SHIFT_H
#define BARREL_SHIFT_H

#include <systemc.h>

SC_MODULE(barrel_shifter){
	//inputs and outputs
	sc_in<bool> clk;				//clock
	sc_in<bool> en;					//enable
	sc_in<bool> l_r;				//shift left or right
	sc_in<sc_uint<3> > shift_amt;	//shift amount
	sc_in<sc_int<8> > d_in;			//data in
	sc_out<sc_int<8> > d_out;		//data out
	
	//function for barrel shifter behaviour, defined in .cpp file
	void behaviour();
	
	//constructor
	SC_CTOR(barrel_shifter){
		SC_METHOD(behaviour);	//use SC_METHOD to synthesize barrel shifter behaviour
		sensitive << clk.pos();	//behaviour called when clock changes to low (positive clock edge)
	}
};
#endif
