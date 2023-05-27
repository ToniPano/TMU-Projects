#include <iostream>
#include <systemc.h>

//import modules
#include "cpa.h"
#include "csa.h"

int sc_main(int argc, char* argv[]){
	//signals
	sc_signal<bool> A, B, Cin, Sum_in, Sum_cpa, Cout_cpa, Sum_csa, Cout_csa;
	
	//initialize carry propagate adder module
	cpa cpa1("cpa1");
	cpa1.A(A);
	cpa1.B(B);
	cpa1.C(Cin);
	cpa1.So(Sum_cpa);
	cpa1.Co(Cout_cpa);
	
	//initialize carry save adder module
	csa csa1("csa1");
	csa1.A(A);
	csa1.B(B);
	csa1.C(Cin);
	csa1.S(Sum_in);
	csa1.So(Sum_csa);
	csa1.Co(Cout_csa);
	
	//set up waveform trace file
	sc_trace_file *tf;
	tf = sc_create_vcd_trace_file("trace_file");	//create trace file called "trace_file.vcd"
	tf -> set_time_unit(1, SC_NS); //set unit time of traces to 1 ns
	sc_trace(tf, A, "A");
	sc_trace(tf, B, "B");
	sc_trace(tf, Cin, "Cin");
	sc_trace(tf, Sum_in, "Sum_in");
	sc_trace(tf, Sum_csa, "Sum_csa");
	sc_trace(tf, Cout_csa, "Cout_csa");
	sc_trace(tf, Sum_cpa, "Sum_cpa");
	sc_trace(tf, Cout_cpa, "Cout_cpa");
	
	//simulation of test cases
	A.write(0);
	B.write(0);
	Cin.write(0);
	sc_start(10, SC_NS);
	
	A.write(1);
	B.write(0);
	Cin.write(0);
	sc_start(10, SC_NS);
	
	A.write(0);
	B.write(1);
	Cin.write(0);
	sc_start(10, SC_NS);
	
	A.write(1);
	B.write(1);
	Cin.write(0);
	sc_start(10, SC_NS);
	
	A.write(0);
	B.write(0);
	Cin.write(1);
	sc_start(10, SC_NS);
	
	A.write(1);
	B.write(0);
	Cin.write(1);
	sc_start(10, SC_NS);
	
	A.write(0);
	B.write(1);
	Cin.write(1);
	sc_start(10, SC_NS);
	
	A.write(1);
	B.write(1);
	Cin.write(1);
	sc_start(10, SC_NS);
	
	//turn on sum input for csa
	Sum_in.write(1);
	A.write(0);
	B.write(0);
	Cin.write(0);
	sc_start(10, SC_NS);
	
	A.write(1);
	B.write(0);
	Cin.write(0);
	sc_start(10, SC_NS);
	
	A.write(0);
	B.write(1);
	Cin.write(0);
	sc_start(10, SC_NS);
	
	A.write(1);
	B.write(1);
	Cin.write(0);
	sc_start(10, SC_NS);
	
	A.write(0);
	B.write(0);
	Cin.write(1);
	sc_start(10, SC_NS);
	
	A.write(1);
	B.write(0);
	Cin.write(1);
	sc_start(10, SC_NS);
	
	A.write(0);
	B.write(1);
	Cin.write(1);
	sc_start(10, SC_NS);
	
	A.write(1);
	B.write(1);
	Cin.write(1);
	sc_start(10, SC_NS);
	
	//included to make sure end of waveform is visible. GTKWave sometimes cuts the end off for some reason
	A.write(0);
	B.write(0);
	Cin.write(0);
	sc_start(10, SC_NS);
}
