#ifndef CSA_H
#define CSA_H

#include "cpa.h"

//carry save adder
SC_MODULE(csa){
	//inputs and outputs
	sc_in<bool> A;		//input bit A
	sc_in<bool> B;		//input bit B
	sc_in<bool> C;		//carry in bit
	sc_in<bool> S;		//sum in bit
	sc_out<bool> So;	//sum of A and B
	sc_out<bool> Co;	//carry out
	
	sc_signal<bool> AB;
	
	//internal carry propagate adder module
	cpa cpa1;

	//behaviour of carry save adder
	void behaviour();
	
	//constructor
	SC_CTOR(csa) : A("A"), B("B"), C("C"), S("S"), Co("Co"), So("So"), cpa1("cpa")
	{
		//set up internal module
		cpa1.A(AB);		//output from AND gate is first input bit
		cpa1.B(S);		//sum acts as other input bit
		cpa1.C(C);
		cpa1.So(So);
		cpa1.Co(Co);
		
		//determine simulation method
		SC_METHOD(behaviour);	//use SC_METHOD to simulate CSA behaviour
		sensitive << A << B;	//call CSA behaviour when any of the inputs change
	}

};

#endif
