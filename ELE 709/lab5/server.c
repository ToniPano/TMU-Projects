#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <mqueue.h>

const char *serverReturnName = "/q1ToniPano";
const char *clientRequestName = "/q2ToniPano";
char message[10];

//computes the nth prime inefficiently
int compute_prime(int n){
    int candidate;
    int factor;
    int is_prime;
    
    candidate = 2;  //2 is the smallest prime number, so start candidate there
    while(1){
        is_prime = 1;
        //test if candidate is prime by seeing if all possible factors do not divide it
        for(factor = 2; factor < candidate; ++factor){
            if(candidate % factor == 0){
                is_prime = 0;
                break;
            }
        }
        
        //if candidate is prime, decrease n. return candidate if it is the nth prime found
        if(is_prime){
            if(--n == 0){
                return candidate;
            }
        }
        
        //otherwise go to the next candidate
        ++candidate;
    }
}

int main(){
    mqd_t serverReturn, clientRequest;
    int oflags;
    mode_t mode;
    struct mq_attr attr;
    unsigned int priority;
    int ret, n, prime;    
    
    mode = S_IRUSR | S_IWUSR;   //set to allow read/write permission in all threads created by user
    attr.mq_maxmsg = 10;       //queue can hold a max of 10 messages
    attr.mq_msgsize = sizeof(message);      //each message in queue is max of 10 bytes    
    
    oflags = O_CREAT | O_WRONLY;  //set to create queue and enable write permission
    serverReturn = mq_open(serverReturnName, oflags, mode, &attr);
    if(serverReturn < 0) perror("serverReturn mq_open() in server:");
    
    oflags = O_CREAT | O_RDONLY;  //set to create queue and enable read permission
    clientRequest = mq_open(clientRequestName, oflags, mode, &attr);
    if(clientRequest < 0) perror("clientReturn mq_open() in server:");
    
    while(1){
        ret = mq_receive(clientRequest, message, (size_t)sizeof(message), &priority);
        if(ret <= 0) continue; //skip to next message if receiving failed or current message is empty
        
        n = atoi(message);  //read number chars in message string and convert them to an int value
        
        //stop if received message is the number 0 (there is no 0th prime number)
        if(n == 0){
            printf("Received a 0, closing server\n");
            break;
        }
        
        prime = compute_prime(n);   //calculate the nth prime number
        sprintf(message, "%d", prime); //store prime number in message as string
        
        ret = mq_send(serverReturn, message, (size_t)strlen(message), priority);
        if(ret < 0) perror("serverReturn mq_send() in server:");
    }
    
    //close both message queues
    mq_close(serverReturn);
    mq_close(clientRequest);
    
    //unlink from message queues so they can be destroyed later
    mq_unlink(serverReturnName);
    mq_unlink(clientRequestName);   
    
    pthread_exit(NULL);
}
