#include <stdio.h>      //printf()
#include <stdlib.h>     //exit()
#include <string.h>     //memset(), strncpy(), strcat()
//#include <sys/socket.h> //socket(), bind(), listen(), accept(), AF_INET, struct sockaddr, struct sockaddr_in
#include <arpa/inet.h>  //htons(), htonl(), ntohs(), ntohl(), INADDR_ANY, plus everything from <sys/socket.h>?
#include <unistd.h>     //fork(), close(), read(), write(), getcwd(), fstat()
#include <sys/stat.h>	//stat struct for fstat()
#include <fcntl.h>      //open(), close(), for files?

#include "lab5_PDU.h"	//defines data packet structure and packet types

#define SERVER_TCP_PORT 3000	/* well-known port */
#define BUFLEN 256
#define CWDLEN 200
#define FOLDERLEN 20

int main(int argc, char **argv){
    int port, ip_address;                   //port number and ip address of server
    struct sockaddr_in server;  //struct for server IP version, IP address, port #
    struct sockaddr_in client;  //struct for client IP version, IP address, port #
    int clientLen = sizeof(client); //weird, but needed in this format for accept()
    int sd;              //UDP socket

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
    sd = socket(AF_INET, SOCK_DGRAM, 0);   //create software socket for UDP/IPv4
    if(sd == -1){
        printf("Can't create a socket\n");
        exit(-1);
    }

    /*bind socket to a port and IP address*/
    memset(&server, 0, sizeof(server));                             //clear server struct contents
    server.sin_family = AF_INET;                                    //set protocol to IPv4
    server.sin_port = port;                                  //specify port to use
    server.sin_addr.s_addr = ip_address;                     //specify IP address of machine running this program, htonl converts host bytes to network byte order in long
    if(bind(sd, (struct sockaddr *)&server, sizeof(server)) == -1){ //bind socket to an available physical port in hardware of the Network Internet Card
        printf("Can't bind port to socket\n");
        exit(-1);
    }

    /*use socket for listening to TCP connection requests, queue up to 5 requests at a time*/
    listen(sd, 5);   //put socket in passive (server) mode to listen for TCP connection requests (buffer size of 5 requests at a time)

    char ip_str[16];    //stores temporary ip address as string for debugging
	struct pdu pkt;
	int n;              //number of bytes that have been read in from socket sd
    int fd;             //file descriptor
    char cwd[CWDLEN];   //current working directory of server program
	char folder[FOLDERLEN] = "/server_files/";	//directory of server files
    char fullFilePath[CWDLEN+FOLDERLEN+sizeof(pkt)];  //full path to file (cwd+fileName)
	struct stat fileInfo;
	int bytesToRead;
	
    while(1){       //loops until child exits or server is externally closed
        /*wait for response from client*/
        recvfrom(sd, &pkt, sizeof(pkt), 0, (struct sockaddr *)&client, &clientLen);//blocks until TCP request comes from a client, assigns new socket descriptor for server to talk to that client
        inet_ntop(AF_INET, &(client.sin_addr.s_addr), ip_str, INET_ADDRSTRLEN); //convert ip address number into string
        printf("received pdu from client!\n\t-type\t= %c\n\t-port number\t= %d\n\t-ip address\t= %s\n", pkt.type, ntohs(client.sin_port), ip_str);
		
		if(pkt.type == FILENAME_PDU){
			//send message to client
			printf("file that client wants is \"%s\"\n", pkt.data);
			
			/*get current working directory, store in full file path*/
			getcwd(cwd, sizeof(cwd));
			strncpy(fullFilePath, cwd, CWDLEN);
			printf("cwd of server is \"%s\"\n", cwd);

			/*add folder to end of "cwd" in full file path*/
			strcat(fullFilePath, folder);

			/*add file name to end of "cwd/folder" in full file path*/
			strcat(fullFilePath, pkt.data);
			printf("full file path is \"%s\"\n", fullFilePath);

			/*search for file*/
			fd = open(fullFilePath, O_RDONLY);
			if(fd == -1){
				printf("Can't open file!\n");
				pkt.type = ERROR_PDU;
				sendto(sd, &pkt, sizeof(pkt), 0, (struct sockaddr *)&client, sizeof(client));
			}
			else{
				fstat(fd, &fileInfo);	//get info about file
				bytesToRead = fileInfo.st_size;	//get total size of file in bytes
				printf("Opened file of size %d bytes!\n", bytesToRead);
				
				pkt.type = DATA_PDU;
				while(bytesToRead > 0){
					n = read(fd, pkt.data, sizeof(pkt.data));
					bytesToRead = bytesToRead-n;
					
					if(bytesToRead == 0){
						pkt.type = FINAL_PDU;
					}
					
					sendto(sd, &pkt, n+1, 0, (struct sockaddr *)&client, sizeof(client));
					printf("Sent %d bytes to client in type %c pdu\n", n+1, pkt.type);
				}
			}
		}
		else{
			printf("pdu is unrecognized\n");
		}
    }
}