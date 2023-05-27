#include <stdio.h>      //printf()
#include <stdlib.h>     //exit()
#include <string.h>     //memset()
//#include <sys/socket.h> //socket(), bind(), listen(), accept(), AF_INET, struct sockaddr, struct sockaddr_in
#include <arpa/inet.h>  //htons(), htonl(), ntohs(), ntohl(), INADDR_ANY, plus everything from <sys/socket.h>?
#include <unistd.h>     //fork(), close(), read(), write()


#define SERVER_TCP_PORT 3000	/* well-known port */
#define BUFLEN 256

int sayHello(int sd);   //function declaration for saying hello to a client

int main(int argc, char **argv){
    int port, ip_address;                   //port number and ip address of server
    struct sockaddr_in server;  //struct for server IP version, IP address, port #
    struct sockaddr_in client;  //struct for client IP version, IP address, port #
    int clientLen = sizeof(client); //weird, but needed in this format for accept()
                            //file descriptors for sockets, aka "socket descriptor"
    int listen_sd;              //passive TCP socket for listening to new TCP requests from client
    int new_sd;                 //active TCP socket with connection to/from client

    //user input for port number
    switch(argc){   //note that first argument is name of object file
        case 1:     //use default if no port given as second argument
            port = htons(SERVER_TCP_PORT);
            ip_address = htonl(INADDR_ANY);//specify IP address of machine running this program, htonl converts host bytes to network byte order in long
            break;
        case 2:             //use port given in second argument
            port = htons(atoi(argv[1]));   //convert string to port # int in network byte order, htons converts host byte order to network byte order in short
            ip_address = htonl(INADDR_ANY);//specify IP address of machine running this program, htonl converts host bytes to network byte order in long
            break;
        default:    //more arguments than expected should trigger a warning and use case of this code
            printf("Usage: %s [port]\n", argv[0]);
            exit(-1);
    }

    /*create socket*/
    listen_sd = socket(AF_INET, SOCK_STREAM, 0);   //create software socket for TCP/IPv4
    if(listen_sd == -1){
        printf("Can't create a socket\n");
        exit(-1);
    }

    /*bind socket to a port and IP address*/
    memset(&server, 0, sizeof(server));                             //clear server struct contents
    server.sin_family = AF_INET;                                    //set protocol to IPv4
    server.sin_port = port;                                  //specify port to use
    server.sin_addr.s_addr = ip_address;                     //specify IP address of machine running this program, htonl converts host bytes to network byte order in long
    if(bind(listen_sd, (struct sockaddr *)&server, sizeof(server)) == -1){ //bind socket to an available physical port in hardware of the Network Internet Card
        printf("Can't bind port to socket\n");
        exit(-1);
    }

    /*use socket for listening to TCP connection requests, queue up to 5 requests at a time*/
    listen(listen_sd, 5);   //put socket in passive (server) mode to listen for TCP connection requests (buffer size of 5 requests at a time)

    char ip_str[16];    //stores temporary ip address as string
    while(1){       //loops until child exits or server is externally closed
        /*get new sd to talk directly to client*/
        new_sd = accept(listen_sd, (struct sockaddr *)&client, &clientLen);//blocks until TCP request comes from a client, assigns new socket descriptor for server to talk to that client
        inet_ntop(AF_INET, &(client.sin_addr.s_addr), ip_str, INET_ADDRSTRLEN); //convert ip address number into string
        printf("connected to client!\n\t-port number\t= %d\n\t-ip address\t= %s\n", ntohs(client.sin_port), ip_str);

        switch(fork()){
            case 0:                 //child of parent
                close(listen_sd);   //close passive socket for listening to TCP requests from other clients
                sayHello(new_sd);   //send message to client
                close(new_sd);      //close active TCP socket connected to client
                exit(0);
            default:                //parent
                close(new_sd);      //close active socket for responding to a specific client's TCP connection
                break;
            case -1:                //fork returns error code
                printf("fork: error\n");
        }
    }
}

int sayHello(int sd){               //sends "Hello" message to client using established TCP connection file descriptor in sd
    char message[BUFLEN] = "Hello";  //message saying "Hello" to client
    write(sd, message, BUFLEN);
    return 0;
}
