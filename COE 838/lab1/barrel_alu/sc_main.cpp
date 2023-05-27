#include <systemc.h>

//modules
#include "barrel_shifter.h"
#include "alu.h"


int sc_main(int argc, char* argv[]){
	//signals
	sc_clock clk("clk", 10, SC_NS, 0.5);	//main clock signal (10 ns period, 50% duty cycle, 0 offset, initial value is 0)
	sc_signal<bool> en, l_r, op;
	sc_signal<sc_uint<3> > shift_amt;
	sc_signal<sc_int<8> > A, B, B_shifted, output;
	
	//barrel shifter module initialization and signal routing
	barrel_shifter barrel("barrel-shifter");
	barrel.clk(clk);
	barrel.en(en);
	barrel.l_r(l_r);
	barrel.shift_amt(shift_amt);
	barrel.d_in(B);
	barrel.d_out(B_shifted);
	
	//arithmetic logic unit module initialization and signal routing
	alu alu("arithmetic-logic-unit");
	alu.clk(clk);
	alu.A(A);
	alu.B(B_shifted);
	alu.op(op);
	alu.R(output);
	
	//setup waveform trace file
	sc_trace_file *tf;
	tf = sc_create_vcd_trace_file("trace_file");	//create trace file named "trace_file.vcd"
	tf -> set_time_unit(1, SC_NS);	//set unit time of trace file to 1 ns
	sc_trace(tf, clk, "clk");	
	sc_trace(tf, A, "A");
	sc_trace(tf, B, "B");
	sc_trace(tf, en, "BS_en");
	sc_trace(tf, l_r, "BS_shift_dir");
	sc_trace(tf, shift_amt, "BS_shift_amt");	
	sc_trace(tf, B_shifted, "BS_out");
	sc_trace(tf, op, "ALU_op");
	sc_trace(tf, output, "ALU_out");
	
	//simulation test sequence
	cout <<	"test enable off with all barrel shifter settings" << endl;
	A.write(1);			//test enable off with all barrel shifter settings
	B.write(-127);	
	en.write(0);
	op.write(0);
	l_r.write(0);
	shift_amt.write(0);
	sc_start(7, SC_NS);
	
	l_r.write(1);
	sc_start(10, SC_NS);
	
	l_r.write(0);
	shift_amt.write(1);
	sc_start(10, SC_NS);
	
	l_r.write(1);
	sc_start(10, SC_NS);
	
	cout << "test enable on, barrel shift both dirs, shift by 1 bit, alu subtract" << endl;	
	en.write(1);		//test enable on, barrel shift both dirs, shift by 1 bit, alu subtract
	l_r.write(0);
	shift_amt.write(0);
	sc_start(10, SC_NS);
	
	l_r.write(1);
	sc_start(10, SC_NS);
	
	l_r.write(0);
	shift_amt.write(1);
	sc_start(10, SC_NS);
	
	l_r.write(1);
	sc_start(10, SC_NS);
		
	cout << "test enable on, barrel shift both dirs, shift by 1 bit, alu add" << endl;
	en.write(1);		//test enable on, barrel shift both dirs, shift by 1 bit, alu add
	l_r.write(0);
	shift_amt.write(0);
	op.write(1);
	sc_start(10, SC_NS);
	
	l_r.write(1);
	sc_start(10, SC_NS);
	
	l_r.write(0);
	shift_amt.write(1);
	sc_start(10, SC_NS);
		
	l_r.write(1);
	sc_start(10, SC_NS);
	
	A.write(0);			//test enable off with all barrel shifter settings
	B.write(0);	
	en.write(0);
	op.write(0);
	l_r.write(0);
	shift_amt.write(0);
	sc_start(13, SC_NS);
			
	//stop simulation
	sc_close_vcd_trace_file(tf);
	
	return 0;
}
