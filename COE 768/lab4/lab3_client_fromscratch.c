#include <stdio.h>      //printf()
#include <stdlib.h>     //exit()
#include <arpa/inet.h>
#include <unistd.h>     //close(), read(), write()

#define SERVER_TCP_PORT 3000	/* well-known port */
#define BUFLEN		256	/* buffer length */
int main(int argc, char* argv[]){
    struct sockaddr_in server;
    int sd, port, ip_address;

    //select IP address and host
    switch(argc){
        case 2:
            inet_pton(AF_INET, argv[1], (void *)&ip_address);   //convert ip address in string to number, using IPv4 protocol format
            if(ip_address <= 0){
                printf("Can't convert string into ip address\n");
                exit(-1);
            }
            break;
        case 3:
            inet_pton(AF_INET, argv[1], (void *)&ip_address);   //convert ip address in string to number, using IPv4 protocol format
            if(ip_address <= 0){
                printf("Can't convert string into ip address\n");
                exit(-1);
            }
            port = htons(atoi(argv[2]));
            break;
        default:
            printf("usage: %s [ip address] [port number]\n", argv[0]);
            exit(-1);
    }

    /*create a TCP socket struct*/
    sd = socket(AF_INET, SOCK_STREAM, 0);   //AF_INET = IPv4, SOCK_STREAM = TCP, 0 = use single default protocol if possible
    if(sd == -1){
        printf("Can't create a socket\n");
        exit(-1);
    }

    /*connect to the server*/
    server.sin_family = AF_INET;
    server.sin_port = port;
    server.sin_addr.s_addr = ip_address;
    if(connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1){
        printf("Can't connect\n");
        exit(-1);
    }
    printf("Connected to server!\n\t-port number\t= %s\n\t-ip address\t= %s\n", argv[2], argv[1]);

    int n;  //amount of bytes that read put into the read buffer string
    char rbuf[BUFLEN];  //the read buffer string
    printf("Receive:\n");
    while((n = read(sd, rbuf, BUFLEN)) > 0){
        write(1, rbuf, n);
    }
    printf("\n");

    close(sd);
}