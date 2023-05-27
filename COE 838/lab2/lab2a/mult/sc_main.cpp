#include <iostream>
#include <math.h>
#include <systemc.h>

//import modules
#include "cpa.h"
#include "csa.h"

const int bits = 3; //min size is 2 bits

int read_sig_arr(sc_signal<bool> sig[bits]){
	int i;
	int sum = 0;
	cout << "N = ";
	for(i = bits-1; i >= 0; i--){
		cout << sig[i].read();
		sum += (sig[i].read() << i);
	}
	cout << " = " << sum << endl;
	
	return sum;
}

void write_sig_arr(sc_signal<bool> sig[bits], int N){
	int i;
	//cout << "N = " << N << " = ";
	for(i = bits-1; i >= 0; i--){
		//cout << ((N & (1 << i)) > 0);
		sig[i].write((N & (1 << i)) > 0);
	}
	//cout << endl;
}

void replace_char(char * str, int pos, char c){
	str[pos] = c;
}

int sc_main(int argc, char* argv[]){
	
	//signals
	sc_signal<bool> A[bits];
	sc_signal<bool> B[bits];
	sc_signal<bool> P [2*bits];
	sc_signal<bool> C_out;
	
	//initialize carry save adder modules (CSA)
	csa* csa_arr [bits][bits];
	sc_signal<bool> csa_Co [bits][bits];
	sc_signal<bool> csa_So [bits][bits];
	int i,j;
	char csa_name [9] = "csa_A#B#";
	cout << "1" << endl;
	
	//connect A B inputs and So Co outputs for each CSA unit
	for(i = 0; i < bits; i++){
		for(j = 0; j < bits; j++){
			replace_char(csa_name, 5, (i+'0'));
			replace_char(csa_name, 7, (j+'0'));
			csa_arr[i][j] = new csa(csa_name);
			csa_arr[i][j] -> A(A[i]);
			csa_arr[i][j] -> B(B[j]);
			if(i > 0){
				csa_arr[i][j] -> So(csa_So[i][j]);
			}
			csa_arr[i][j] -> Co(csa_Co[i][j]);
		}
	}
	cout << "2" << endl;
	
	//S = 0 at edge of matrix B[0]
	sc_signal<bool> csa_S_B0 [bits];
	for(i = 0; i < bits; i++){
		csa_arr[i][0] -> S(csa_S_B0[i]);
		csa_S_B0[i].write(0);
	}
	cout << "3" << endl;
	
	//S = 0 at edge of matrix A[MSB]
	sc_signal<bool> csa_S_AMSB[bits-1];
	for(j = 1; j < bits; j++){
		csa_arr[bits-1][j] -> S(csa_S_AMSB[j-1]);
		csa_S_AMSB[j-1].write(0);
	}
	cout << "4" << endl;
	
	//S = S of previous B bit and next A bit: for B[1] to B[MSB] and A[0] to A[MSB-1]
	for(i = 0; i < bits-1; i++){
		for(j = 1; j < bits; j++){
			csa_arr[i][j] -> S(csa_So[i+1][j-1]);
		}
	}
	cout << "5" << endl;
	
	//C = 0 at edge of matrix B[0] 
	sc_signal<bool> csa_C_B0 [bits];
	for(i = 0; i < bits; i++){
		csa_arr[i][0] -> C(csa_C_B0[i]);
		csa_C_B0[i].write(0);
	}
	cout << "6" << endl;
	
	//C = (C of previous B bit) in each A row
	for(i = 0; i < bits; i++){
		for(j = 1; j < bits; j++){
			csa_arr[i][j] -> C(csa_Co[i][j-1]);
		}
	}
	cout << "7" << endl;
	
	//connect So of each B bit in row A[0] to P[0:bits-1]
	for(j = 0; j < bits; j++){
		csa_arr[0][j] -> So(P[j]);
	}
	cout << "8" << endl;
	
	//initialize carry propagate adder modules (CPA) for each A bit
	cpa* cpa_arr [bits];
	sc_signal<bool> cpa_Co [bits];	
	char cpa_name [7] = "cpa_A#";
	for(i = 0; i < bits; i++){
		replace_char(cpa_name, 5, (i+'0'));
		cpa_arr[i] = new cpa(cpa_name);
		if(i < bits-1){
			cpa_arr[i] -> Co(cpa_Co[i]);
			cpa_arr[i] -> B(csa_Co[i][bits-1]);	//connect B of each CPA to Co of each CSA (in B[MSB] row for each A bit)
		}
		cpa_arr[i] -> So(P[i+bits]);		//connect So of each CPA to each P bit
	}
	cout << "9" << endl;
	
	//connect Co of prev CPA to C of next CPA
	//connect So of next CSA (in B[MSB] row) to A of prev CPA for CPA 0 and CPA 1
	for(i = 0; i < bits-1; i++){
		if(i < bits-2){
			cpa_arr[i+1] -> C(cpa_Co[i]);	//connect
		}
		cpa_arr[i] -> A(csa_So[i+i][bits-1]);
	}
	cout << "10" << endl;
	
	//set C of first CPA to 0
	sc_signal<bool> cpa_C0;
	cpa_arr[0] -> C(cpa_C0);
	cpa_C0.write(0);
	cout << "11" << endl;
	
	//for last CPA, set A to Co of last CSA, set B to Co of prev CPA, set C to 0, and set Co to C_out 
	cpa_arr[bits-1] -> A(csa_Co[bits-1][bits-1]);
	cpa_arr[bits-1] -> B(cpa_Co[bits-2]);
	sc_signal<bool> cpa_CMSB;
	cpa_arr[bits-1] -> C(cpa_CMSB);
	cpa_CMSB.write(0);
	cpa_arr[bits-1] -> Co(C_out);
	cout << "12" << endl;
	
	//set up waveform trace file
	sc_trace_file *tf;
	tf = sc_create_vcd_trace_file("trace_file");	//create trace file called "trace_file.vcd"
	tf -> set_time_unit(1, SC_NS); //set unit time of traces to 1 ns
	char trace_name [5] = "A[#]";
	for(i = 0; i < bits; i++){
		replace_char(trace_name, 2, (i+'0'));
		sc_trace(tf, A[i], "A"+i);
	}
	trace_name[0] = 'B';
	for(i = 0; i < bits; i++){
		replace_char(trace_name, 2, (i+'0'));
		sc_trace(tf, B[i], "B"+i);
	}
	trace_name[0] = 'P';
	for(i = 0; i < 2*bits; i++){
		replace_char(trace_name, 2, (i+'0'));
		sc_trace(tf, P[i], "P"+i);
	}
	sc_trace(tf, C_out, "C_out");
	cout << "13" << endl;
	
	//simulation of test cases
	int a, b, p;
	for(i = 0; i < pow(2, bits); i++){
		for(j = 0; j < pow(2, bits); j++){
	
			write_sig_arr(A, i);
			write_sig_arr(B, j);
			sc_start(10, SC_NS);
			a = read_sig_arr(A);
			b = read_sig_arr(B);
			p = read_sig_arr(P);
			cout << "A * B = C_out P: " << a << " * " << b << " = " << C_out.read() << " " << p << endl;
		}
	}
	
	cout << "14" << endl;
}
