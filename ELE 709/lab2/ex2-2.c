#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int i;
    int maxitr = 500000000; //5*10^8
    struct timespec clock_res, TIME1, TIME2;
    double a, b, c, ELAPSED_TIME, ITR_TIME;
    char op, run;
    
    printf("This program calculates the average elapsed time for a math operation on two doubles.\n");
    printf("The four possible math operations are:\n\t+ (addition)\n\t- (subtraction)\n\t* (multiplication)\n\t/ (division)\n");
    
    //ask user if they would like to enter another operation
    printf("Enter a math operation? (y/n):\n");
    scanf("%c", &run);
    
    while(run == 'y' || run == 'Y'){
        getchar(); //remove ENTER key from input stream
        printf("Please enter a number (num), a math operator sign (op), and another number (num) on the same line in the form of: num op num\n");    
        //using scanf to skip whitespace between numbers and operator sign
        scanf("%lf %c %lf", &a, &op, &b);
        printf("a = %lf, op = %c, b = %lf\n", a, op, b);
        
        //in case user did not enter a known operation sign
        while(op != '+' && op != '-' && op != '*' && op != '/'){
            printf("Please use a math operation sign (+, -, *, or /) between the numbers:\n");
            //using scanf to skip whitespace between numbers and operator sign
            scanf("%lf %c %lf", &a, &op, &b);
            printf("a = %lf, op = %c, b = %lf\n", a, op, b);
        }
        
        getchar(); //get rid of ENTER key in input stream
        printf("\n");
        
        //display real time clock period
        clock_getres(CLOCK_REALTIME, &clock_res);
        printf("Resolution of CLOCK %d is %d sec and %d nsec\n", CLOCK_REALTIME, clock_res.tv_sec, clock_res.tv_nsec);
        
        /*//The double operations in this code take too long, makes ITR_TIME above 45 ns
        //In comparison the lab suggestions make ITR_TIME stay around 3.07 ns
        ELAPSED_TIME = 0;
        for(i = 0; i < maxitr; i++){
            clock_gettime(CLOCK_REALTIME, &TIME1);        
            c = a + b;
            clock_gettime(CLOCK_REALTIME, &TIME2);
            ELAPSED_TIME += (TIME2.tv_sec-TIME1.tv_sec)*1e9 + (TIME2.tv_nsec-TIME1.tv_nsec);
        }*/
        
        if(op == '+'){
            clock_gettime(CLOCK_REALTIME, &TIME1);
            for(i = 0; i < maxitr; i++) c = a + b; //addition
            clock_gettime(CLOCK_REALTIME, &TIME2);
        }
        else if(op == '-'){
            clock_gettime(CLOCK_REALTIME, &TIME1);
            for(i = 0; i < maxitr; i++) c = a - b; //subtraction
            clock_gettime(CLOCK_REALTIME, &TIME2);
        }
        else if(op == '*'){
            clock_gettime(CLOCK_REALTIME, &TIME1);
            for(i = 0; i< maxitr; i++) c = a * b; //multiplication
            clock_gettime(CLOCK_REALTIME, &TIME2);
        }
        else if(op == '/'){
            clock_gettime(CLOCK_REALTIME, &TIME1);
            for(i = 0; i< maxitr; i++) c = a / b; //division
            clock_gettime(CLOCK_REALTIME, &TIME2);
        }
        else{
            printf("Unrecognized operation character '%c'\n", op);
            return 0;
        }
        
        ELAPSED_TIME = (TIME2.tv_sec-TIME1.tv_sec)*1e9 + (TIME2.tv_nsec-TIME1.tv_nsec);
        
        ITR_TIME = ELAPSED_TIME/maxitr;//calculate average elapsed time for a single iteration of the selected operation
        printf("Time per iteration is %lf nsec\n", ITR_TIME);
        
        //ask user if they would like to enter another operation
        printf("Enter another operation? (y/n):\n");
        scanf("%c", &run);
    }
    printf("program done\n");       
    return 0;
}
