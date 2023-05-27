#ifndef SPLITTER_H
#define SPLITTER_H

#include <systemc.h>

//split int into array of ports
template <int BITS>
SC_MODULE(splitter){
	sc_in<sc_uint<BITS> > in;
	sc_vector< sc_out<bool> > out;
	
	int i, num;
	
	//splits input into each port for output
	void behaviour();
	
	SC_CTOR(splitter) : out("out", BITS)
	{
		SC_METHOD(behaviour);
		sensitive << in;
	}
};


template <int BITS>
void splitter <BITS> :: behaviour(){
	num = in.read();
	
	for(i = 0; i < BITS; i++){
		out[i].write((num & (1 << i)) > 0);
	}
}

#endif
