#ifndef JOINER_H
#define JOINER_H

#include <systemc.h>

//split int into array of ports
template <int BITS>
SC_MODULE(joiner){
	sc_vector< sc_in<bool> > in;
	sc_out< sc_uint<BITS> > out;
	
	int i, sum;
	
	//splits input into each port for output
	void behaviour();
	
	SC_CTOR(joiner) : in("in", BITS)
	{
		SC_METHOD(behaviour);
		for(i = 0; i < in.size(); i++){
			sensitive << in[i];
		}
	}
};

template <int BITS>
void joiner <BITS> :: behaviour(){
	sum = 0;
	for(i = 0; i < BITS; i++){
		sum += (in[i].read() << i);
	}
	out.write(sum);
}

#endif
