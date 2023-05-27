#include <stdio.h>      //printf()
#include <stdlib.h>     //exit()
#include <string.h>		//memset(), strncpy(), strcat()
#include <arpa/inet.h>
#include <unistd.h>     //close(), read(), write()
#include <fcntl.h>      //open(), close(), for files?

#include "lab5_PDU.h"

#define SERVER_TCP_PORT 3000	/* well-known port */
#define BUFLEN		256	/* buffer length */
#define CWDLEN 		200
#define FOLDERLEN	20
#define FILELEN		99

void getFilePath(char fileName[FILELEN], char * fullFilePath);	//function declaration for getting absolute file path of new file to create

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

    /*create a UDP socket struct*/
    sd = socket(AF_INET, SOCK_DGRAM, 0);   //AF_INET = IPv4, SOCK_STREAM = UDP, 0 = use single default protocol if possible
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

    int n;  			//amount of bytes that read put into the read buffer string
    struct pdu pkt;	//data packet to be reused for storing transmissions
	char fullFilePath[CWDLEN+FOLDERLEN+FILELEN];  //full path to file (cwd/folder/fileName)
	int fd;				//file descriptor to created file
	
    /*get file name from user*/
    pkt.type = FILENAME_PDU;
    printf("File name: ");
    scanf(" %s", pkt.data);     //note that scanf LEAVES WHITESPACE IN THE STDIN BUFFER! it will mess up the next attempt to use scanf by making it parse that whitespace and return nothing if it is an end of line character
	
	/*get full file path to specified file name in "/client_files/" folder in current working directory*/
	getFilePath(pkt.data, fullFilePath);
	
	/*create file*/
    fd = open(fullFilePath, O_CREAT|O_WRONLY|O_TRUNC|S_IRWXU);	//O_CREAT = create file, O_WRONLY = write only, O_TRUNC = overwrite any existing file, S_IRWXU = create file
    if(fd == -1){
        printf("Can't create file!\n");
        exit(-1);
    }
	
	/*send file name to server*/
    write(sd, &pkt, sizeof(pkt));
	printf("Sent filename to server in type %c pdu\n", pkt.type);
	
    /*receive file info from server*/
    printf("Received:\n");
    while((n = read(sd, &pkt, sizeof(pkt))) > 0){
        printf("pdu of type = %c ", pkt.type);
        //exit if server sends error or unknown packet type
        if(pkt.type == DATA_PDU){
            printf("(data,\t%d bytes)\n", n);
            write(fd, pkt.data, n-1);   //write to file (n includes packet type byte, n-1 excludes it)
        }
		else if(pkt.type == FINAL_PDU){
			printf("(final,\t%d bytes)\n", n);
            write(fd, pkt.data, n-1);   //write to file (n includes packet type byte, n-1 excludes it)
			close(fd);	//close file
			break;
		}
        else if(pkt.type == ERROR_PDU){	//prepare to delete created file if error packet
			close(fd);	//close file (must close file before assigning value to fd?)
			fd = -1;
            printf("(error)\n");
			break;
        }
        else{
            printf("(unrecognized)\n");
        }
    }
	
	/*delete file if error packet, otherwise close file normally*/
	if(fd == -1){
		remove(fullFilePath);	//delete file
	}
	
    close(sd);	//close UDP connection to server
}

//function for getting absolute file path of new file to create
void getFilePath(char fileName[FILELEN], char * fullFilePath){
	char cwd[CWDLEN];   //current working directory of client program
	char folder[FOLDERLEN] = "/client_files/";	//folder for client files
	
    /*get current working directory, store in full file path*/
    getcwd(cwd, sizeof(cwd));
	strncpy(fullFilePath, cwd, CWDLEN);
    printf("cwd of client is \"%s\"\n", cwd);
	
	/*add folder to end of "cwd" in full file path*/
	strcat(fullFilePath, folder);
	
    /*add file name to end of cwd, store in full file path*/
    strcat(fullFilePath, fileName);
    printf("full file path is \"%s\"\n", fullFilePath);
}