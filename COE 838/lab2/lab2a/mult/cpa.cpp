#include <iostream>
#include "cpa.h"

//internal signals
bool a, b, c_in, sum, c_out;
bool a_xor_b;

bool XOR(bool x, bool y);

void cpa :: behaviour(){
	//read inputs
	a = A.read();
	b = B.read();
	c_in = C.read();
	
	//calculate sum and carry out signals
	sum = XOR(c_in, XOR(a, b));								//C_in xor A xor B
	c_out = ((!c_in) && (a && b)) + (c_in && (a + b));	//[NOT(C_in) AND (A AND B)] OR [C_in AND (A OR B)]
	
	//set outputs
	So.write(sum);
	Co.write(c_out);
	
	//cout << name() << " [Cin A B] = " << c_in << " " << a << " " << b << " -> [Cout Sum]: " << c_out << " " << sum << endl;
}

bool XOR(bool x, bool y){
	return ((!x) && y) || (x && (!y));
}
