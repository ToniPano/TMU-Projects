#include <stdio.h>      //printf()
#include <stdlib.h>     //exit()
#include <string.h>     //memset(), strncpy(), strcat()
//#include <sys/socket.h> //socket(), bind(), listen(), accept(), AF_INET, struct sockaddr, struct sockaddr_in
#include <arpa/inet.h>  //htons(), htonl(), ntohs(), ntohl(), INADDR_ANY, plus everything from <sys/socket.h>?
#include <unistd.h>     //fork(), close(), read(), write(), getcwd()
#include <fcntl.h>      //open(), close(), for files?
#include <ctype.h>		//toupper()
#include <sys/select.h> //select()
#include <sys/time.h>

#include "pdu.h"	//defines data packet structure and packet types

#define BUFLEN	256
#define CWDLEN	200
#define FILELEN	100

void getFilePath(char fileName[FILELEN], char * fullFilePath);	//function declaration for getting absolute file path of new file to create
int sendFile(int sd);   //function declaration for sending file data to a peer client

int main(int argc, char **argv){
	char ip_str[16];	//temporary string for displaying ip with periods
	char localhost_str[10] = "localhost"; //string which user types to use ip address of local host
	
	int sockaddr_in_len = sizeof(struct sockaddr_in);	//weird, but needed in this format for accept()
	int index_server_ip, index_server_port;	//ip and port of index server from user input 
	int this_server_ip, this_server_port;	//ip and port of 
	
	struct sockaddr_in server;	//struct reused for servers, has IP version, IP address, port #
	struct sockaddr_in client;	//struct reused for clients, has IP version, IP address, port #
                            //file descriptors for sockets, aka "socket descriptor"
    int index_server_sd;	//active UDP connectionless connection to index server
	int listen_sd;          //passive TCP socket for listening to new TCP requests from peer clients
	int peer_server_sd;		//active TCP socket for downloading as client from another peer as server
	int peer_client_sd;     //active TCP socket with connection to/from a server client
	
	fd_set rfds, afds;
	
	/*create passive TCP socket*/
    listen_sd = socket(AF_INET, SOCK_STREAM, 0);   //create software socket for TCP/IPv4
    if(listen_sd == -1){
        printf("Can't create a socket\n");
        exit(-1);
    }
	
	FD_ZERO(&afds);
	FD_SET(listen_sd, &afds); //listening on a TCP socket
	FD_SET(0, &afds); //listening on stdin
	memcpy(&rfds, &afds, sizeof(rfds));

    /*bind passive TCP socket to a port and IP address*/
    memset(&server, 0, sizeof(server));   //clear server struct contents
    server.sin_family = AF_INET;               //set protocol to IPv4
	server.sin_port = htons(0);						//0 means bind to any available port
    server.sin_addr.s_addr = htonl(INADDR_ANY);//specify IP address of machine running this program, htonl converts host bytes to network byte order in long
    if(bind(listen_sd, (struct sockaddr *)&server, sizeof(server)) == -1){ //bind socket to an available physical port in hardware of the Network Internet Card
        printf("Can't bind port to socket\n");
        exit(-1);
    }

    /*use passive TCP socket for listening to TCP connection requests fro  other peers, queue up to 5 requests at a time*/
    listen(listen_sd, 5);   //put socket in passive (server) mode to listen for TCP connection requests (buffer size of 5 requests at a time)
	
	/*get port and ip address of the passive TCP connection*/
	getsockname(listen_sd, (struct sockaddr *)&server, &sockaddr_in_len);		//get port and ip of passive socket
	this_server_port = server.sin_port;
	this_server_ip = server.sin_addr.s_addr;
	inet_ntop(AF_INET, &this_server_ip, ip_str, INET_ADDRSTRLEN); //convert ip address number into string
    printf("Created passive TCP socket:\n\tport number\t= %d\n\tip address\t= %s\n", ntohs(this_server_port), ip_str);
	
    /*get ip address and port number of index server from user input*/
    switch(argc){
        case 3:
			if(strcmp(argv[1], localhost_str) == 0){
				index_server_ip = htonl(INADDR_ANY);
				//printf("using localhost ip = %d\n", index_server_ip);
			}
			else{
				inet_pton(AF_INET, argv[1], (void *)&index_server_ip);   //convert ip address in string to number, using IPv4 protocol format
				if(index_server_ip <= 0){
					printf("Can't convert string into ip address\n");
					exit(-1);
				}
			}
			
            index_server_port = htons(atoi(argv[2]));
            break;
        default:
            printf("usage: %s [index server ip address] [index server port number]\n", argv[0]);
            exit(-1);
    }
	
	/*create a UDP socket struct to connect to index server*/
    index_server_sd = socket(AF_INET, SOCK_DGRAM, 0);   //AF_INET = IPv4, SOCK_STREAM = UDP, 0 = use single default protocol if possible
    if(index_server_sd == -1){
        printf("Can't create a socket\n");
        exit(-1);
    }

    /*connect to index server*/
    memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
    server.sin_port = index_server_port;
    server.sin_addr.s_addr = index_server_ip;
    if(connect(index_server_sd, (struct sockaddr *)&server, sizeof(server)) == -1){
        printf("Can't connect\n");
        exit(-1);
    }
    printf("Created UDP socket to index server at:\n\tport number\t= %s\n\tip address\t= %s\n", argv[2], argv[1]);
	
	/*main loop*/
	struct pdu pkt;	//stores packets sent to /received from server. Variable is reused (overwritten) for each packet
	char peer_name[10];		//stores name of this peer
	char content_name[10]; //store names, etc, typed into user input
	char reg_contents[20][10]; //store list of content registered on server
	int reg_content_ptr = 0;
	char fullFilePath[CWDLEN+FILELEN];  //full path to file (cwd/folder/fileName)
	int fd;				//file descriptor to created file
	char input[10];	//store input from scanf()
	char command;	//store user's menu choice
	int loop = 1;
	int i, n;
	
	/*get name of peer*/
	printf("\nEnter the name of this peer: ");
	scanf("%s", peer_name);//read user's command, note that scanf leaves whitespace in the stdin buffer
	printf("peer named to \"%s\"\n", peer_name);
	
	printf("\nList of commands (uppercase/lowercase does not matter):\n\t'L' = list of local content in peer\n\t'R' = content registration\n\t'O' = list available content from index server\n\t'S' = search for specific content\n\t'T' = content deregistration\n\t'Q' = quit program\n");
	
	//get user input
	printf("Enter a letter to do a command: ");
	
	while(select(FD_SETSIZE, &rfds, NULL, NULL, NULL) > 0){
		
		if(FD_ISSET(0, &rfds)){	//stdin (0) has new input to read
			scanf("%s", input); //read user's command, note that scanf leaves whitespace in the stdin buffer
			command = toupper(input[0]);	//convert any input char to uppercase
			
			//check user input cases
			switch(command){
				case '?':	//help user by listing menu commands and re-reading user input
					printf("List of commands (uppercase/lowercase does not matter):\n\t'L' = list of local content in peer\n\t'R' = content registration\n\t'O' = list available content from index server\n\t'S' = search for specific content\n\t'T' = content deregistration\n\t'Q' = quit program\n");
					break;
					
				case 'L':	//list local content
					system("ls");	//call "ls" command from Linux terminal via system call
					break;
					
				case reg_pdu:	//'R' = register one specific content
					//send r type pdus until successful
					while(1){
						printf("Enter the name of your content to register: ");
						scanf("%s", content_name);
						
						pkt.type = command;
						strncpy(pkt.data, peer_name, 10);
						strncpy(&(pkt.data[10]), content_name, 10);
						*((int *) &(pkt.data[20])) = this_server_ip;
						*((short *) &(pkt.data[24])) = this_server_port;
						
						write(index_server_sd, &pkt, sizeof(pkt));	//send R type pdu to server
						read(index_server_sd, &pkt, sizeof(pkt));	//read pdu that server sent in response
						
						if(pkt.type == err_pdu){//check if peer name is in use
							if(pkt.data[0] == 'D'){
								printf("Duplicate entry found. No changes made to server list.\n");
								break;
							}
							else{
								printf("Error: %s\n", pkt.data);
								printf("Name \"%s\" is in use by another peer on the index server\nWould you like to use another name? (type 'y' for yes or any char for no): ", peer_name);
								scanf("%s", input);
								command = toupper(input[0]);// convert any input chr to uppercase
								if(command == 'y' || command == 'Y'){
									printf("Enter a new name for this peer: ");
									scanf("%s", peer_name);//read user's command, note that scanf leaves whitespace in the stdin buffer
								}
								else break;
							}
						}
						else if(pkt.type == ack_pdu){
							printf("Successfully registered content \"%s\" from peer \"%s\"\n", content_name, peer_name);
							
							//save registered content name in array for deregistering content during quitting
							strncpy(reg_contents[reg_content_ptr], content_name, 10);
							if(reg_content_ptr < 20){
								reg_content_ptr++;
							}
							break;
						}
						else{
							printf("Received unknown pdu of type %c\n", pkt.type);
							break;
						}
					}
					break;
					
				case list_pdu:	//'O' = list all content on index server
					memset(&pkt, 0, sizeof(pkt)); //clear packet contents
					pkt.type = command; //set packet type to 'O' for list type
					write(index_server_sd, &pkt, sizeof(pkt)); //send list type packet to index server
					read(index_server_sd, &pkt, sizeof(pkt)); //receive list of content from server
					
					//assumes content name is max. of 10 bytes long
					printf("List of content on index server:\n");
					printf("--- peer_name content_name ip port---\n");
					
					while(1){
						//printf("Received pdu of type %c\n", pkt.type);
						if(pkt.type == ack_pdu) break;
						
						inet_ntop(AF_INET, ((int *) &(pkt.data[20])), ip_str, INET_ADDRSTRLEN);
						printf("\t%s\t%s\t%d\t%d\n", &(pkt.data[0]), &(pkt.data[10]), *((int *) &(pkt.data[20])), ntohs(*((int *) &(pkt.data[24]))));
						read(index_server_sd, &pkt, sizeof(pkt));
					}
					break;
					
				case search_pdu:	//'S' = download one specific content
					printf("Enter the name of the content you want to download: ");
					scanf("%s", content_name);
					
					pkt.type = command;
					strncpy(pkt.data, peer_name, 10);
					strncpy(&(pkt.data[10]), content_name, 10);
					
					write(index_server_sd, &pkt, sizeof(pkt));
					read(index_server_sd, &pkt, sizeof(pkt));
					
					if(pkt.type == err_pdu){
						printf("Error: %s\n", pkt.data);
						printf("No content named \"%s\" found in index server\n", content_name);
						break;
					}
					else if(pkt.type == search_pdu){
						inet_ntop(AF_INET, &(pkt.data[10]), ip_str, INET_ADDRSTRLEN); //convert ip address number into string
						printf("Successfully found content named \"%s\" in index server\n", content_name);
						
						/*create a TCP socket struct*/
						peer_server_sd = socket(AF_INET, SOCK_STREAM, 0);   //AF_INET = IPv4, SOCK_STREAM = TCP, 0 = use single default protocol if possible
						if(peer_server_sd == -1){
							printf("Can't create a socket\n");
							exit(-1);
						}
						
						//char string[7];
						//memcpy(string, &(pkt.data[10]), 7);

						/*connect to another peer offering its content as a server*/
						server.sin_family = AF_INET;
						server.sin_port = *((short*)&(pkt.data[24])); //port as short from type S packet of chars
						server.sin_addr.s_addr = *((int*)&(pkt.data[20])); //ip address as int from type S packet of chars
						
						inet_ntop(AF_INET, &(server.sin_addr.s_addr), ip_str, INET_ADDRSTRLEN); //convert ip address number into string
						printf("trying to create TCP socket to peer server:\n\tport number\t= %d\n\tip address\t= %s\n", server.sin_port, ip_str);
						
						if(connect(peer_server_sd, (struct sockaddr *)&server, sizeof(server)) == -1){
							printf("Can't connect\n");
							exit(-1);
						}
						
						//create local file to store downloaded content
						getFilePath(&(pkt.data[10]), fullFilePath); //get full absolute file path to location of this program file
						FILE * ptr  = fopen(fullFilePath,"w");
						if(ptr == NULL)
						printf("CANNOT CREATE FILE \n");
						
						fd =1;
						
						//fd = open(fullFilePath, O_CREAT|O_WRONLY|O_TRUNC|S_IRWXU);	//O_CREAT = create file, O_WRONLY = write only, O_TRUNC = overwrite any existing file, S_IRWXU = create file
						if(fd == -1){
							printf("Can't create file!\n");
							exit(-1);
						}
						
						//download content from peer content server
						pkt.type = dload_pdu;
						strncpy(&(pkt.data[10]), content_name, 10);
						
						write(peer_server_sd, &pkt, sizeof(pkt)); //send D type packet to peer content server
						
						printf("Received:\n");
						while((n = read(peer_server_sd, &pkt, sizeof(pkt))) > 0){
							printf("\tpacket of type = %c ", pkt.type);
							//exit if server sends error or unknown packet type
							if(pkt.type == data_pdu){
								printf("(data, %d bytes)\n", n);
								//write(fd, pkt.data, n-1);   //write to file (n includes packet type byte, n-1 excludes it)
								fwrite(pkt.data,1,n-1,ptr);
								printf("\n");
							}
							else if(pkt.type == err_pdu){	//prepare to delete created file if error packet
								//close(fd);
								fclose(ptr);
								fd = -1;
								printf("(error)\n");
							}
							else{
								printf("(unrecognized)\n");
							}
						}
						
						
						//delete file if error packet, otherwise close file normally
						if(fd == -1){
							remove(fullFilePath);	//delete file
						}
						else{
							printf("Closing File \n");
							fclose(ptr);
					
							//close(fd);				//close file
						}
						
						close(peer_server_sd); //close active TCP connection to peer content server
						
						//register this peer as the new content server for the recently downloaded content
						pkt.type = reg_pdu;
						strncpy(pkt.data, peer_name, 10);
						strncpy(&(pkt.data[10]), content_name, 10);
						*((int *) &(pkt.data[20])) = this_server_ip;
						*((int *) &(pkt.data[24])) = this_server_port;
						
						write(index_server_sd, &pkt, sizeof(pkt));	//send R type pdu to server
						read(index_server_sd, &pkt, sizeof(pkt));	//read pdu that server sent in response
						
						if(pkt.type == err_pdu){//check if peer name is in use
							printf("Error: %s", pkt.data);
							printf("Name \"%s\" is in use by another peer on the index server\n", peer_name);
						}
						else if(pkt.type == ack_pdu){
							printf("Successfully registered content \"%s\" from peer \"%s\"\n", content_name, peer_name);
						}
						else{
							printf("Received unknown pdu of type %c\n", pkt.type);
						}
					}
					else{
						printf("Received unknown pdu of type %c\n", pkt.type);
					}
					break;
					
				case dereg_pdu:	//'T' = deregister one specific content
					printf("Enter the name of your content to deregister: ");
					scanf("%s", content_name);
					
					pkt.type = command;
					strncpy(pkt.data, peer_name, 10);
					strncpy(&(pkt.data[10]), content_name, 10);
					*((int *) &(pkt.data[20])) = this_server_ip;
					*((int *) &(pkt.data[24])) = this_server_port;
					
					write(index_server_sd, &pkt, sizeof(pkt));	//send T type pdu to server
					read(index_server_sd, &pkt, sizeof(pkt));	//read pdu that server sent in response
					
					if(pkt.type == ack_pdu){
						printf("Successfully deregistered content \"%s\" from peer \"%s\"\n", content_name, peer_name);
					}
					else if(pkt.type == err_pdu){
						printf("Error: %s\n", pkt.data);
						printf("No content named \"%s\" registered with peer \"%s\" in index server\n", content_name, peer_name);
					}
					break;
					
				case 'Q': //deregister all remaining content and quit
					//send T type packet for every registered piee of content from this peer
					pkt.type = dereg_pdu;
					for(i = reg_content_ptr; i > 0; i--){
						strncpy(pkt.data, reg_contents[i-1], 10);
						
						write(index_server_sd, &pkt, sizeof(pkt));	//send T type pdu to server
						read(index_server_sd, &pkt, sizeof(pkt));	//read pdu that server sent in response
						
						if(pkt.type == ack_pdu){
							printf("Successfully deregistered content \"%s\" from peer \"%s\"\n", content_name, peer_name);
						}
						else if(pkt.type == err_pdu){
							printf("Error: %s\n", pkt.data);
							printf("No content named \"%s\" registered with peer \"%s\" in index server\n", content_name, peer_name);
						}
					}
					
					printf("Closing connection to index server\n");
					close(index_server_sd); //close UDP connectionless connection
					printf("peer \"%s\" closed\n", peer_name);
					exit(0);
				case 'D':
				
				printf("Received TCP connection from peer client\n");
			peer_client_sd = accept(listen_sd, (struct sockaddr *) &client, &sockaddr_in_len);
			
			//fork to let child process deal with TCP request, and parent process to stay in main loop
			switch(fork()){
				case 0:	//child process
					close(listen_sd);	//close passive TCP socket
					sendFile(peer_client_sd);	//send file with content to peer client
					close(peer_client_sd); //close TCP socket to peer client after download is done
					exit(0);
				default: //parent process
					close(peer_client_sd); //close active TCP socket to peer client asking for download
					FD_ZERO(&afds);
					FD_SET(listen_sd, &afds); //listening on a TCP socket
					memcpy(&rfds, &afds, sizeof(rfds));
					break;
				case -1:                //fork returns error code
					printf("fork: error\n");
					exit(-1);
			}
					
				default:
					printf("command '%c' unrecognized. Type '?' for help with commands.\n", command);
					break;
			}
			
			//get user input
			printf("Enter a letter to do a command: ");
			
			//register stdin for select() again?
			FD_ZERO(&afds);
			FD_SET(0, &afds); //listening on stdin
			memcpy(&rfds, &afds, sizeof(rfds));
		}
		
		if(FD_ISSET(listen_sd, &rfds)){ //listen_sd has at least 1 new connection request
			printf("Received TCP connection from peer client\n");
			peer_client_sd = accept(listen_sd, (struct sockaddr *) &client, &sockaddr_in_len);
			
			//fork to let child process deal with TCP request, and parent process to stay in main loop
			switch(fork()){
				case 0:	//child process
					close(listen_sd);	//close passive TCP socket
					sendFile(peer_client_sd);	//send file with content to peer client
					close(peer_client_sd); //close TCP socket to peer client after download is done
					exit(0);
				default: //parent process
					close(peer_client_sd); //close active TCP socket to peer client asking for download
					FD_ZERO(&afds);
					FD_SET(listen_sd, &afds); //listening on a TCP socket
					memcpy(&rfds, &afds, sizeof(rfds));
					break;
				case -1:                //fork returns error code
					printf("fork: error\n");
					exit(-1);
			}
		}
	}
}

//function for getting absolute file path of new file to create
void getFilePath(char fileName[FILELEN], char * fullFilePath){
	char cwd[CWDLEN];   //current working directory of client program
	char slash[2] = "/";
	
    /*get current working directory, store in full file path*/
    getcwd(cwd, sizeof(cwd));
	strncpy(fullFilePath, cwd, CWDLEN);
    printf("cwd of client is \"%s\"\n", cwd);
	
	/*add slash after cwd*/
	strcat(fullFilePath, slash);
	
    /*add file name to end of cwd, store in full file path*/
    strcat(fullFilePath, fileName);
    printf("full file path is \"%s\"\n", fullFilePath);
}

//reads file contents to peer client
int sendFile(int sd){
    int n;              //number of bytes that have been read in from socket sd
    struct pdu pkt;  //data packet struct
    int fd;             //file descriptor
    char cwd[CWDLEN];   //current working directory of server program
	char slash [2] = "/";
    char fullFilePath[CWDLEN+sizeof(pkt)];  //full path to file (cwd+fileName)

    /*get current working directory, store in full file path*/
    getcwd(cwd, sizeof(cwd));
	strncpy(fullFilePath, cwd, CWDLEN);
    printf("cwd of server is \"%s\"\n", cwd);
	
	/*add slash after cwd*/
	strcat(fullFilePath, slash);
	
	/*get file name from client*/
    read(sd, &pkt, sizeof(pkt));
    printf("file that client wants is \"%s\"\n", &(pkt.data[10]));
	
    /*add file name to end of cwd in full file path*/
    strcat(fullFilePath, &(pkt.data[10]));
    printf("full file path is \"%s\"\n", fullFilePath);

    /*search for file*/
    fd = open(fullFilePath, O_RDONLY);
    if(fd == -1){
        printf("Can't open file!\n");
        pkt.type = err_pdu;
        write(sd, &pkt, sizeof(pkt));
        close(fd);
        return -1;
    }
    else{
        printf("Opened file!\n");
        pkt.type = data_pdu;
        while((n = read(fd, pkt.data, sizeof(pkt.data))) > 0){
            write(sd, &pkt, n+1);
            printf("Sent %d bytes to client\n", n+1);
        }
        close(fd);
        return 0;
    }
}
