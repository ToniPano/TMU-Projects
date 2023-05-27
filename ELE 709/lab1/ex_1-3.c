#include <stdio.h>

int main(){
	double sum, theta[100];
	int i;
	
	for(i = 0; i < 100; i++){
		theta[i] = i + 1;
	}
	
	sum = 0.0;
	i = 0;
	while(i < 100){
		sum = sum + theta[i];
		i = i + 1;
	}
	printf("The sum of the array is %lf\n", sum);
}
