#include <stdio.h>

#define ONE_THIRD 1.0/3.0
/*
  ONE_THIRD is expected to give the following result:
	2.0/ONE_THIRD = 2.0/(1.0/3.0) = 2.0*3.0/1.0 = 6.0/1.0 = 6.0

  However the compiler doesn't recognize 1.0/3.0 as a single number, and attempts to
  apply BEDMAS order of operations to our syntax of 1.0/3.0:
  	2.0/ONE_THIRD = 2.0/1.0/3.0 = (2.0/1.0)/3.0 = (2.0)/3.0 = 2.0/3.0 = 0.6666...7

  Therefore ONE_THIRD requires brackets around 1.0/3.0 in order to work correctly:
	#define ONE_THIRD (1.0/3.0)
	...
	2.0/ONE_THIRD = 2.0/(1.0/3.0) = 6.0
*/

int main(int argc, char *argv[]){
	double a;
	a = 2.0/ONE_THIRD;
	printf("a is %lf\n", a);
}
