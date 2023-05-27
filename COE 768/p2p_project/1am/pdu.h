#define reg_pdu 	'R' //Content Registration
#define dload_pdu   'D' //Download Request
#define search_pdu  'S' //Search for Content
#define dereg_pdu   'T' //Deregister Content
#define data_pdu 	'C' //Content Data
#define list_pdu 	'O' //Online List of Reg Content
#define ack_pdu  	'A' //Acknowledgement
#define err_pdu 	'E' //Error

struct pdu{
	char type;
	char data[100];
};