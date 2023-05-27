#include <stdio.h>      /* for fprintf() */
#include <stdlib.h>     /* for exit() */
#include <unistd.h>     /* for sleep() */
#include <pthread.h>    /* for pthread functions */

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;

int string_index = 0;
char string_to_print[] = "0123456789";

void *funcA(void *arg)
{
    int tmp_index;
    
    sleep(1);   /* sleep 1 sec to allow all threads to start */
    
    while(1){
        while(string_index % 2 != 0){ //index not even
            pthread_cond_wait(&cond_var, &mutex); //waits to receive signal, then waits to lock mutex
        }
        
        tmp_index = string_index;
        
        if(!(tmp_index+2 > sizeof(string_to_print))){   // tmp_index < sizeof(string_to_print), i.e. index within string bounds
            printf("A%c ", string_to_print[tmp_index]);
            usleep(1);  /* sleep 1 usec */
        }
        
        string_index = tmp_index + 1;
        
        if(tmp_index+1 > sizeof(string_to_print)){
            printf("\n");
            string_index = 0;   /* wrap around */
        }
        
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond_var); // signal that string_index is now odd
    }
}

void *funcB(void *arg)
{
    int tmp_index;
    
    sleep(1);   /* sleep 1 sec to allow all threads to start */
    
    while(1){
        while(string_index % 2 == 0){ //index not odd
            pthread_cond_wait(&cond_var, &mutex); //waits to receive signal, then waits to lock mutex
        }
        
        tmp_index = string_index;
        
        if(!(tmp_index+2 > sizeof(string_to_print))){   // tmp_index < sizeof(string_to_print), i.e. index within string bounds
            printf("B%c ", string_to_print[tmp_index]);
            usleep(1);  /* sleep 1 usec */
        }
        
        string_index = tmp_index + 1;
        
        if(tmp_index+1 > sizeof(string_to_print)){
            printf("\n");
            string_index = 0;   /* wrap around */
        }
        
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond_var); // signal that string_index is now even
    }
}

int main(void)
{
    pthread_t threadA, threadB;
    
    printf("Starting Thread A\n");
    if(pthread_create(&threadA, NULL, &funcA, NULL) != 0){
        printf("Error creating thread A\n");
        exit(-1);
    }
    
    printf("Starting Thread B\n");
    if(pthread_create(&threadB, NULL, &funcB, NULL) != 0){
        printf("Error creating thread B\n");
        exit(-1);
    }
    
    sleep(2);  /* sleep 20 secs to allow time for the threads to run */
                /* before terminating them with pthread_cancel()      */
    
    pthread_cancel(threadA);
    pthread_cancel(threadB);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);
    //pthread_exit(NULL);
}
