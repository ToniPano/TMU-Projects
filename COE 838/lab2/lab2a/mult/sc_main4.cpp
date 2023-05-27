#include <iostream>
#include <math.h>
#include <systemc.h>

//import modules
#include "cpa.h"
#include "csa.h"
#include "mult.h"

void test_cpa_csa();
void test_mult();

int sc_main(int argc, char* argv[]){
	//set up multipler unit
	const int bits = 8; //min size is 2 bits
	sc_signal< sc_uint<bits> > a("a");
	sc_signal< sc_uint<bits> > b("b");
	sc_signal< sc_uint<2*bits> > p("p");
	
	mult<bits> m("mult");
	m.a(a);
	m.b(b);
	m.p(p);
	
	//set up waveform trace file
	sc_trace_file *tf_mult;
	tf_mult = sc_create_vcd_trace_file("trace_file");	//create trace file called "trace_file_mult.vcd"
	tf_mult -> set_time_unit(1, SC_NS); //set unit time of traces to 1 ns
	sc_trace(tf_mult, a, "a");
	sc_trace(tf_mult, b, "b");
	sc_trace(tf_mult, p, "p");
	
	//simulation of test cases
	int cases = 5;
	int arr[5] = {0,1,27,254,255};
	int i, j;
	for(i = 0; i < cases; i++){
		for(j = 0; j < cases; j++){
			a.write(arr[i]);
			b.write(arr[j]);
			sc_start(10, SC_NS);
			cout << "a * b = p: " << a.read() << " * " << b.read() << " = " << p.read() << endl;
		}
	}
	
	//pass end with 10 more seconds, otherwise last test case is hard to see in waveform file
	a.write(0);
	b.write(0);
	sc_start(10, SC_NS);
	sc_close_vcd_trace_file(tf_mult);
}
