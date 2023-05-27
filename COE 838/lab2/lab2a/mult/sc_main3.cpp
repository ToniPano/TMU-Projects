#include <iostream>
#include <math.h>
#include <systemc.h>

//import modules
#include "cpa.h"
#include "csa.h"
#include "splitter.h"
#include "joiner.h"

int sc_main(int argc, char* argv[]){
	const int bits = 8; //min size is 2 bits
	
	//signals, including input splitters and output joiner modules
	sc_signal< sc_uint<bits> > a;
	sc_vector< sc_signal<bool> > A("A", bits);
	splitter<bits> sa ("splitter_a");
	sa.in(a);
	sa.out(A);
	
	sc_signal< sc_uint<bits> > b;
	sc_vector< sc_signal<bool> > B("B", bits);
	splitter<bits> sb ("splitter_b");
	sb.in(b);
	sb.out(B);
	
	sc_vector< sc_signal<bool> > P ("P", 2*bits);
	sc_signal< sc_uint<2*bits> > p;
	joiner <2*bits> jp ("joiner_p");
	jp.in(P);
	jp.out(p);
	
	sc_signal<bool> C_out;
	
	//initialize carry save adder modules (CSA)
	csa* csa_arr [bits][bits];
	sc_signal<bool> csa_Co [bits][bits];
	sc_signal<bool> csa_So [bits][bits];
	sc_signal<bool> csa_C_B0 [bits];
	sc_signal<bool> csa_S_B0 [bits];
	sc_signal<bool> csa_S_AMSB[bits-1];
	int i,j;
	char csa_name [9] = "csa_A#B#";
	cout << "1" << endl;
	
	for(j = 0; j < bits; j++){
		for(i = 0; i < bits; i++){
			csa_name[5] = i + '0';
			csa_name[7] = j + '0';
			cout << csa_name << " ";
			csa_arr[i][j] = new csa(csa_name);
			csa_arr[i][j] -> A(A[i]);
			csa_arr[i][j] -> B(B[j]);
			csa_arr[i][j] -> Co(csa_Co[i][j]);
			
			if(i == 0){
				csa_arr[0][j] -> So(P[j]); //connect So of each B bit in row A[0] to P[0:bits-1]
			}
			else{
				csa_arr[i][j] -> So(csa_So[i][j]);
			}
			
			//assigning 0 connections to C and S
			if(j == 0){
				csa_arr[i][0] -> C(csa_C_B0[i]);	//C = 0 at edge of matrix B[0]
				csa_C_B0[i].write(0);
				csa_arr[i][0] -> S(csa_S_B0[i]);	//S = 0 at edge of matrix B[0]
				csa_S_B0[i].write(0);
			}
			else{
				csa_arr[i][j] -> C(csa_Co[i][j-1]);	//C = (C of previous B bit) in each A row
				
				if(i == bits-1){
					csa_arr[bits-1][j] -> S(csa_S_AMSB[j-1]);	//S = 0 at edge of matrix A[MSB]
					csa_S_AMSB[j-1].write(0);
				}
				else{
					csa_arr[i][j] -> S(csa_So[i+1][j-1]);	//S = S of previous B bit and next A bit: for B[1] to B[MSB] and A[0] to A[MSB-1]
				}
			}
		}
		cout << endl;
	}
	cout << "2" << endl;
	
	//initialize carry propagate adder modules (CPA) for each A bit
	cpa* cpa_arr [bits];
	sc_signal<bool> cpa_Co [bits];
	sc_signal<bool> cpa_AMSB, cpa_C0;
	char cpa_name [7] = "cpa_A#";
	
	for(i = 0; i < bits; i++){
		cpa_name[5] = i + '0';
		cout << " " << cpa_name << "  ";
		cpa_arr[i] = new cpa(cpa_name);
		cpa_arr[i] -> B(csa_Co[i][bits-1]);	//connect B of each CPA to Co of each CSA (in B[MSB] row for each A bit)
		cpa_arr[i] -> So(P[i+bits]);		//connect So of each CPA to each P[4:7] bit
		
		if(i == 0){
			cpa_arr[0] -> C(cpa_C0);				//for first CPA, set C to '0'
			cpa_arr[i] -> A(csa_So[i+1][bits-1]);	//connect So of next CSA (in B[MSB] row) to A of prev CPA
			cpa_arr[i] -> Co(cpa_Co[i]);			//connect Co from each CPA to array of Co signals
			cpa_C0.write(0);
		}
		else if(i == bits-1){
			cpa_arr[i] -> C(cpa_Co[i-1]);	//connect Co of prev CPA to C of next CPA
			cpa_arr[bits-1] -> A(cpa_AMSB);		//for last CPA, set A to '0'
			cpa_arr[bits-1] -> Co(C_out);		//for last CPA, set Co to C_out
			cpa_AMSB.write(0);
		}
		else{
			cpa_arr[i] -> C(cpa_Co[i-1]);			//connect Co of prev CPA to C of next CPA
			cpa_arr[i] -> A(csa_So[i+1][bits-1]);	//connect So of next CSA (in B[MSB] row) to A of prev CPA
			cpa_arr[i] -> Co(cpa_Co[i]);			//connect Co from each CPA to array of Co signals
		}
	}
	cout << endl;
	cout << "3" << endl;
	
	//set up waveform trace file
	sc_trace_file *tf;
	tf = sc_create_vcd_trace_file("trace_file");	//create trace file called "trace_file.vcd"
	tf -> set_time_unit(1, SC_NS); //set unit time of traces to 1 ns
	sc_trace(tf, a, "a");
	sc_trace(tf, b, "b");
	sc_trace(tf, p, "p");
	sc_trace(tf, C_out, "C_out");
	cout << "4" << endl;
	
	//simulation of test cases
	for(i = 0; i < pow(2, bits); i++){
		for(j = 0; j < pow(2, bits); j++){
			a.write(i);
			b.write(j);
			sc_start(10, SC_NS);
			cout << "A * B = C_out P: " << a.read() << " * " << b.read() << " = " << C_out.read() << " " << p.read() << endl;
		}
	}
	
	//pass end with 10 more seconds, otherwise last test case is hard to see in waveform file
	a.write(0);
	b.write(0);
	sc_start(10, SC_NS);
	
	cout << "5" << endl;
}
