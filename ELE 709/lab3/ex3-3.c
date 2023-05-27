#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

struct thread_info {
    double a, b;        // The numbers a and b
    char op;            // The math iteration to be performed: +,-,*,/
    int maxitr;         // The number of iterations to be performed
    double c;           // The result: c = a <op> b
    double exec_time;   // The average execution time per iteration in nsec

};

typedef struct thread_info thread_info_t;

void *func(void *arg)
{
    struct timespec time_1, time_2;
    double exec_time;
    thread_info_t *info;
    int i, maxitr;
    volatile double a, b, c;
    char op;
    
    info = (thread_info_t *) arg;
    a = info->a;
    b = info->b;
    op = info->op;
    maxitr = info->maxitr;
    
    if(op == '+'){
        clock_gettime(CLOCK_REALTIME, &time_1);
        for(i = 0; i < maxitr; i++) c = a + b; //addition
        clock_gettime(CLOCK_REALTIME, &time_2);
    }
    else if(op == '-'){
        clock_gettime(CLOCK_REALTIME, &time_1);
        for(i = 0; i < maxitr; i++) c = a - b; //subtraction
        clock_gettime(CLOCK_REALTIME, &time_2);
    }
    else if(op == '*'){
        clock_gettime(CLOCK_REALTIME, &time_1);
        for(i = 0; i < maxitr; i++) c = a * b; //multiplication
        clock_gettime(CLOCK_REALTIME, &time_2);
    }
    else if(op == '/'){
        clock_gettime(CLOCK_REALTIME, &time_1);
        for(i = 0; i < maxitr; i++) c = a / b; //division
        clock_gettime(CLOCK_REALTIME, &time_2);
    }
    
    info->c = c;
    
    exec_time = (time_2.tv_sec - time_1.tv_sec)*1.0e9;
    exec_time = exec_time + (time_2.tv_nsec - time_1.tv_nsec);
    info->exec_time = exec_time/maxitr;   
    
    pthread_exit(NULL);
}

int main(void){
    pthread_t threadAdd, threadSub, threadMult, threadDiv;
    thread_info_t infoAdd, infoSub, infoMult, infoDiv;
    double a = 2.3, b = 4.5;
    int maxitr = 5e8;
    
    //addition info
    infoAdd.a = a;
    infoAdd.b = b;
    infoAdd.op = '+';
    infoAdd.maxitr = maxitr;
    
    //subtraction info
    infoSub.a = a;
    infoSub.b = b;
    infoSub.op = '-';
    infoSub.maxitr = maxitr;
    
    //multiplication nifo
    infoMult.a = a;
    infoMult.b = b;
    infoMult.op = '*';
    infoMult.maxitr = maxitr;
    
    //division info
    infoDiv.a = a;
    infoDiv.b = b;
    infoDiv.op = '/';
    infoDiv.maxitr = maxitr;
    
    //addition thread
    if(pthread_create(&threadAdd, NULL, &func, &infoAdd) != 0){
        printf("Error in creating Addition Thread\n");
        exit(1);
    }
    
    //subtraction thread
    if(pthread_create(&threadSub, NULL, &func, &infoSub) != 0){
        printf("Error in creating Subtraction Thread\n");
        exit(1);
    }
    
    //multiplication thread
    if(pthread_create(&threadMult, NULL, &func, &infoMult) != 0){
        printf("Error in creating Multiplication Thread\n");
        exit(1);
    }
    
    //division thread
    if(pthread_create(&threadDiv, NULL, &func, &infoDiv) != 0){
        printf("Error in creating Division Thread\n");
        exit(1);
    }
    
    pthread_join(threadAdd, NULL);
    printf("Exec time for + Thread: %lf nsec, c = %lf\n", infoAdd.exec_time, infoAdd.c);
    
    pthread_join(threadSub, NULL);
    printf("Exec time for - Thread: %lf nsec, c = %lf\n", infoSub.exec_time, infoSub.c);
    
    pthread_join(threadMult, NULL);
    printf("Exec time for * Thread: %lf nsec, c = %lf\n", infoMult.exec_time, infoMult.c);
    
    pthread_join(threadDiv, NULL);
    printf("Exec time for / Thread: %lf nsec, c = %lf\n", infoDiv.exec_time, infoDiv.c);
    
    pthread_exit(NULL);
}
