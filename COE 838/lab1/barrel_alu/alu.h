#ifndef ALU_H
#define ALU_H

#include <systemc.h>

SC_MODULE(alu){
	//inputs and outputs
	sc_in<bool> clk;		//clock
	sc_in<sc_int<8> > A;	//input A (8 bits)
	sc_in<sc_int<8> > B;	//input B (8 bits)
	sc_in<bool> op;			//opcode (1 bit)
	sc_out<sc_int<8> > R;	//result (8 bits)
	
	//function for ALU behaviour, defined in .cpp file
	void behaviour();
	
	//constructor
	SC_CTOR(alu){
		SC_METHOD(behaviour);	//use SC_METHOD to synthesize ALU behaviour
		sensitive << clk.neg();	//behaviour called when clock changes to low (negative clock edge)
	}
};
#endif
