#include <iostream>
#include "alu.h"

sc_int<8> result;

void alu :: behaviour(){
	cout << "\tALU op: " << op.read() << ", A: " << A.read() << ", B: " << B.read() << ", ";
	
	if(op.read() == 0){	//subtraction (A-B)
		result = A.read() - B.read();
		cout << "A - B = ";
	}
	else{ //addition (A+B)
		result = A.read() + B.read();
		cout << "A + B = ";
	}
	R.write(result);
	
	cout << result << endl;
}
