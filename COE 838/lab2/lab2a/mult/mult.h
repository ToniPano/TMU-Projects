#ifndef MULT_H
#define MULT_H

#include <systemc.h>

#include <string.h>
#include "cpa.h"		//carry propagte adder (CPA)
#include "csa.h"		//carry save adder (CPA)
#include "splitter.h"	//split single int signal into vector of bit signals
#include "joiner.h"		//join vector of bit signals into single int signal

template<int bits>	//min input width for each input into multiplier is at least 2 bits wide
SC_MODULE(mult){
	//inputs and outputs of multiplier
	sc_in< sc_uint<bits> > a;
	sc_in< sc_uint<bits> > b;
	sc_out< sc_uint<2*bits> > p;
	
	//signals and modules for splitting input bit signals from their input port, and joining output bit signals to their output port
	sc_vector< sc_signal<bool> > A;
	splitter<bits> sa;
	
	sc_vector< sc_signal<bool> > B;
	splitter<bits> sb;
	
	sc_vector< sc_signal<bool> > P;
	joiner<2*bits> jp;
	
	//carry save adder (CSA) modules and signals
	csa* csa_arr [bits][bits];
	sc_signal<bool> csa_Co [bits][bits];
	sc_signal<bool> csa_So [bits][bits];
	sc_signal<bool> csa_C_B0 [bits];
	sc_signal<bool> csa_S_B0 [bits];
	sc_signal<bool> csa_S_AMSB[bits-1];
	int i,j;
	char csa_name [9];
	
	//carry propagate adder (CPA) modules and signals
	cpa* cpa_arr [bits];
	sc_signal<bool> cpa_Co [bits];
	sc_signal<bool> cpa_AMSB, cpa_C0, C_out; 
	char cpa_name [7];
	
	SC_CTOR(mult) : sa("splitter_a"), A("A", bits), sb("splitter_b"), B("B", bits), P("P", 2*bits), jp("joiner_p")
	{
		//splitter and joiner blocks linked to signals and input/output ports
		sa.in(a);
		sa.out(A);
		sb.in(b);
		sb.out(B);
		jp.in(P);
		jp.out(p);
		
		cout << "mult structure:" << endl;
		
		//initialize carry save adder modules (CSA)
		strncpy(csa_name, "csa_A#B#", 9);
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
		
		//initialize carry propagate adder modules (CPA) for each A bit
		strncpy(cpa_name, "cpa_A#", 6);
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
		
		cout << "done setup of mult unit" << endl;
	}
};

#endif
