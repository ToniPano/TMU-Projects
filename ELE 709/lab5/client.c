#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pthread.h>
#include <mqueue.h>

const char *serverReturnName = "/q1ToniPano";
const char *clientRequestName = "/q2ToniPano";
char message[10];
unsigned int priority = 1;
int stop_printing = 0;


void* prompt(void* arg){
    int ret;
    //open client request message queue in write only mode
    mqd_t clientRequest = mq_open(clientRequestName, O_WRONLY);
    if(clientRequest == -1){
        perror("clientRequest mq_open() in client_prompt:");
    }
    
    stop_printing = 0;
    int n;
    while(1){
        //prompt user for non-negative number
        n = -1;
        while(n < 0){
            printf("Enter a non-negative number n to get the nth prime number: \n");
            scanf("%d", &n);
        }
        
        //convert number to string and send to clent request message queue
        sprintf(message, "%d", n);
        ret = mq_send(clientRequest, message, (size_t)sizeof(message), priority);
        if(ret < 0) perror("clientRequest mq_send() in client_prompt");
        
        //stop prompting user for input and sending messages to server if n is 0
        if(n == 0){
            printf("Client_prompt saw the number was 0. Stopping prompt\n");
            mq_close(clientRequest);
            stop_printing = 1;
            pthread_exit(NULL);
        }
    }
}

void* print(void* arg){
    struct mq_attr attr;
    int ret;
    //open server return message queue in read only mode
    mqd_t serverReturn = mq_open(serverReturnName, O_RDONLY);
    if(serverReturn == -1){
        perror("serverReturn mq_open() in client_print:");
    }
    
    while(1){
        //get number of messages currently in server message queue
        mq_getattr(serverReturn, &attr);
        
        //only check message queue if it is not empty (otherwise it blocks until a message has been found) 
        if(attr.mq_curmsgs > 0){
            //get prime number from server return message queue
            printf("Client_prompt waiting for message from serverReturn message queue\n");
            ret = mq_receive(serverReturn, message, (size_t)sizeof(message), &priority);
            if(ret <= 0) continue; //skip to next message if receiving failed or current message is empty
            printf("prime number = %d\n", atoi(message));
        }
        
        //check if printing should stop
        if(stop_printing == 1){
            printf("Client_print stopped by client_prompt\n");
            mq_close(serverReturn);
            pthread_exit(NULL);
        }
    }
}

int main(){
    pthread_t client_prompt, client_print;
    
    if(pthread_create(&client_prompt, NULL, &prompt, NULL) != 0){
        printf("Error in creating client prompt thread");
        exit(-1);
    }
    
    if(pthread_create(&client_print, NULL, &print, NULL) != 0){
        printf("Error in creating client prompt thread");
        exit(-1);
    }
    
    pthread_join(client_prompt, NULL);
    pthread_join(client_print, NULL);
    
    pthread_exit(NULL);
}
