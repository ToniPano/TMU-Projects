/* time_server.c - main */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <time.h>
#include "pdu.h"

/*------------------------------------------------------------------------
 * main - Iterative UDP server for TIME service
 *------------------------------------------------------------------------
 */
 
 typedef struct list {
   char pname[10], cname[10], add[10];
    struct list * next;
} list_t;
 
int
main(int argc, char *argv[])
{
	struct  sockaddr_in fsin;	/* the from address of a client	*/
	char	buf[100];		/* "input" buffer; any size > 0	*/
	char    *pts;
	int	sock;			/* server socket		*/
	time_t	now;			/* current time			*/
	int	alen;			/* from-address length		*/
	struct  sockaddr_in sin; /* an Internet endpoint address         */
        int     s, type;        /* socket descriptor and socket type    */
	int 	port=3000;
	int flag =0, dup = 0;
	struct pdu client;
	
                                                                                

	switch(argc){
		case 1:
			break;
		case 2:
			port = atoi(argv[1]);
			break;
		default:
			fprintf(stderr, "Usage: %s [port]\n", argv[0]);
			exit(1);
	}

        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_port = htons(port);
                                                                                                 
    /* Allocate a socket */
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (s < 0)
		fprintf(stderr, "can't create socket\n");
                                                                                
    /* Bind the socket */
        if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		fprintf(stderr, "can't bind to %d port\n",port);
        listen(s, 5);	
	alen = sizeof(fsin);

	list_t * head = NULL;
	list_t * current;
	list_t * previous;
	//head = (list_t*) malloc(sizeof(list_t));
	current = (list_t*)malloc(sizeof(list_t));
	 char checkpeer[10],checkcontent[10];
	int checkip;
	short checkport;

	while (1) {
		
		if (recvfrom(s, &client, sizeof(client), 0,
				(struct sockaddr *)&fsin, &alen) < 0) //READ DIRECTLY INTO CLIENT
			fprintf(stderr, "recvfrom error\n");
			
			//client.type = buf[0]; // set the pdu type
			printf("Client Type: %c \n",client.type);
			//memmove(&(buf[0]),&(buf[1]),100);
			//client.data = buf;
			//strcpy(client.data,buf);
		printf("Data in buffer: \n %s \n",&(client.data[10]));
			
			if(client.type == reg_pdu) //Content Registration
			{
				/*
				Check if other peer with same name has registered the content aka reregister
				if yes send error
				else register content into array
				*/
				
				if(head == NULL) // First in List
				{
					printf("Head: %p\n", head);
					head = (list_t*)malloc(sizeof(list_t));
					printf("BEFORE HEAD\n");
					memcpy(head->pname, &(client.data[0]),10); // Copy peer name from buffer to list
					memcpy(head->cname, &(client.data[10]),10); // Copy Content name from buffer to list
					memcpy(head->add, &(client.data[20]),6); // Copy address from buffer to list
					head->next = NULL;
					client.type = ack_pdu;
					(void) sendto(s, &client, sizeof(client), 0,
						(struct sockaddr *)&fsin, sizeof(fsin)); 
					printf("BEFORE BREAK\n");
					printf("Head Set to: %p\n", head);
					//break;
				}
				else{
				printf("List Has Items in it\n");
					 current = head;
					
					 memcpy(checkpeer, &(client.data[0]),10); // Copy peer name from buffer to list
					 memcpy(checkcontent, &(client.data[10]),10); // Copy peer name from buffer to list
					checkip = *((int *) &(client.data[20]));
					checkport = *((short *) &(client.data[24]));
					
					while(current!=NULL)
					 {
						
						if(checkip == *((int *) &(current->add)) && checkport == *((short *) &((current->add)[4]))){ //if both peer name and content are same
							printf("Address (ip = %d, port = %d) matches node\n", checkip, ntohs(checkport));
							
							//duplicate entry found (peer, content, address match)
							if(strcmp(checkpeer,current->pname) == 0 && strcmp(checkcontent, current->cname) == 0)
							{
								printf("Peer name \"%s\" and content name \"%s\" matches node\n", current->pname, current->cname);
								dup = 1;
								break;									
							}
						}
						else{
							
							//addresses different but peer names match means new peer using server. They must choose a different name
							if(strcmp(checkpeer, current->pname) == 0){
								printf("Peer name \"%s\" matches node\n", current->pname);
								flag = 1;
							}
						}
						
						printf("Current: %p\n Next: %p\n", current, current->next);
						previous = current;
						current = current->next;
					 }
					//current->next =(list_t*) malloc(sizeof(list_t)); 
					if(flag == 1){
						printf("Inside Flag  == 1\n");
						flag =0;
						client.type = err_pdu;
						printf("Client Type: %c \n",client.type);
						char err[] = "PEER NAME TAKEN";
						memcpy(&(client.data[0]),err,strlen(err)); //Copy Content peer name into data
						(void) sendto(s, &client, sizeof(client), 0, (struct sockaddr *)&fsin, sizeof(fsin));
					}
					else if(dup == 1){
						printf("Inside dup  == 1\n");
						dup = 0;
						client.type = err_pdu;
						printf("Client Type: %c \n",client.type);
						char err [] = "DUPLICATE ENTRY";
						memcpy(client.data, err, strlen(err));
						(void) sendto(s, &client, sizeof(client), 0, (struct sockaddr *)&fsin, sizeof(fsin));  // Send ack
					}
					else{
						printf("Inside Else\n");
						printf("Current ID before set: %p\n", current);
						current =(list_t*) malloc(sizeof(list_t));
						previous->next = current;	//link this node to previous node
						memcpy(current->pname, &(client.data[0]),10); // Copy peer name from buffer to list
						memcpy(current->cname, &(client.data[10]),10); // Copy Content name from buffer to list
						printf("Before Address\n");
						memcpy(current->add, &(client.data[20]),6); // Copy address from buffer to list
						current->next = NULL;
						printf("Current ID after set: %p\n", current);
						client.type = ack_pdu;
						printf("Client Type: %c \n",client.type);
						(void) sendto(s, &client, sizeof(client), 0, (struct sockaddr *)&fsin, sizeof(fsin));  // Send ack
					}
					
				}
				
			}
			
			if(client.type == search_pdu) //Search content listing for downloadable content
			{
				/*
				Peer contact index server and send S type PDU
				Extract peer name(10b) and content name(10b/end of data field) 
				Check peer name in list and send S with Address if exists and E if doesnt exist
				*/
				//char checkpeer[10]; 
				current = head;
				int miss = 1;
				
				memcpy(checkcontent, &(client.data[10]),10);
				while(current!=NULL){
					if(strcmp(checkcontent,current->cname) == 0)
					{
					 client.type = search_pdu;
					 memcpy(&(client.data[20]),current->add,6); //Copy Content server address into data
					 (void) sendto(s, &client, sizeof(client), 0,
						(struct sockaddr *)&fsin, sizeof(fsin)); // Send data with Search Type and address
						miss = 0;
						break;
					}
				 current = current->next;
				 }
				
				if(miss){
					client.type = err_pdu; //if failed send err pdu
					(void) sendto(s, &client, sizeof(client), 0, (struct sockaddr *)&fsin, sizeof(fsin)); // Send Error
				}
			}
				
			if(client.type == dereg_pdu)
			{ 
				// GO through and remove content with associated name and content
				int miss = 1;			//flag if nothing in list matches query
				list_t * current = head;
				list_t * temp = NULL;	//is current
				list_t * temp2 = NULL;	//is current -> next
				memcpy(checkpeer, &(client.data[0]),10); // Copy peer name from buffer to list
				memcpy(checkcontent, &(client.data[10]),10); // Copy peer name from buffer to list
				
				printf("loop start: current = %p, temp = %p\n", current, temp);
				while(current->next != NULL)
				{
					printf("in loop: current = %p, temp = %p\n", current, temp);
					if(strcmp(checkpeer,current->next->pname) == 0 && strcmp(checkcontent,current->next->cname) == 0)
					{
						temp = current->next;		//node to remove
						current->next = temp->next;	//link node after with node before, avoiding node to remove
						free(temp);					//remove the node we want to remove
						miss = 0;
						printf("Node found that matches peer name \"%s\" and content name \"%s\"\n", checkpeer, checkcontent);
						break;
					}
					
					current = current->next;
				}
				
				if(strcmp(checkpeer, head->pname) == 0 && strcmp(checkcontent, head->cname) == 0){
					printf("Head matches peer name \"%s\" and content name \"%s\"\n", checkpeer, checkcontent);
					temp = head;
					head = head -> next;
					free(temp);
					miss = 0;
				}
				
				if(miss){
					printf("Sending error pdun");
					client.type = err_pdu; //if failed send err pdu
					(void) sendto(s, &client,sizeof(client), 0,
							(struct sockaddr *)&fsin, sizeof(fsin)); // Send Error
				}
				else{
					printf("Sending ack pdu\n");
					client.type = ack_pdu;
					sendto(s, &client, sizeof(client), 0, (struct sockaddr *) &fsin, sizeof(fsin));
				}
			}
			
			
			if(client.type == list_pdu)
			{
				//Compile and send list of registered content
				current = head; // Set Pointer to top of list
				while(current!=NULL)
				{
				
				client.type = list_pdu;
				memcpy(&(client.data[0]),current->pname,10); //Copy Content peer name into data
				memcpy(&(client.data[10]),current->cname,10); //Copy Content name into data
				memcpy(&(client.data[20]),current->add,6); //Copy Content server address into data
				(void) sendto(s, &client, sizeof(client), 0,
						(struct sockaddr *)&fsin, sizeof(fsin));
					
				current = current->next;
				}
				
				printf("Sending ack pdu\n");
				client.type = ack_pdu;
				sendto(s, &client, sizeof(client), 0, (struct sockaddr *)&fsin, sizeof(fsin));
			}
				

		printf("\n");
	}
}