#ifndef CPA_H
#define CPA_H

#include <systemc.h>

//carry propagate adder
SC_MODULE(cpa){
	//inputs and outputs
	sc_in<bool> A;		//input bit A
	sc_in<bool> B;		//input bit B
	sc_in<bool> C;		//carry in bit
	sc_out<bool> So;	//sum of A and B
	sc_out<bool> Co;	//carry out
	
	//behaviour of carry save adder
	void behaviour();
	
	//constructor
	SC_CTOR(cpa) : A("A"), B("B"), C("C"), Co("Co"), So("So")
	{
		SC_METHOD(behaviour);		//use SC_METHOD to simulate CPA behaviour
		sensitive << A << B << C;	//call CPA behaviour when any of the inputs change
	}
};

#endif
