#include <stdio.h>

struct lab {
	double experiment1;
	double experiment2;
	double experiment3;
};

struct theory {
	double test;
	double final;
};

struct ele709_record {
	long ID_number;
	struct lab lab_mark;
	struct theory theory_mark;
};

double total_mark(struct ele709_record *p){
	double total_mark;
	
	total_mark = p->lab_mark.experiment1 + p->lab_mark.experiment2 + p->lab_mark.experiment3;
	total_mark = total_mark * 0.2/3.0;
	
	total_mark = total_mark + (p->theory_mark.test * 0.3);
	
	total_mark = total_mark + (p->theory_mark.final * 0.5);
	
	return total_mark;
}

int main(){
	struct ele709_record john_doe;
	double john_doe_mark;	

	john_doe.ID_number = 12345678;
	john_doe.lab_mark.experiment1 = 90.2;
	john_doe.lab_mark.experiment2 = 70.5;
	john_doe.lab_mark.experiment3 = 80.4;
	john_doe.theory_mark.test = 82.3;
	john_doe.theory_mark.final = 79.2;
	
	john_doe_mark = total_mark(&john_doe);
	
	printf("John Doe's total mark is %lf\n", john_doe_mark);
}
