#include <iostream>
#include "csa.h"

bool a_and_b;

void csa :: behaviour() {
	a_and_b = A.read() & B.read();
	AB.write(a_and_b);	//simulate AND gate at input of CPA adder block
	
	//cout << "\t" << name() << " [Cin A B S] = " << C.read() << " " << A.read() << " " << S.read() << " " << B.read() << ", [Cin A*B S] " << C.read() << " " <<  a_and_b << " " << B.read() << " -> " << endl;
}
