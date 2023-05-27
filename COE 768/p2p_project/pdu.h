#define reg_pdu		'R'	//Content Registration
#define dload_pdu	'D'	//Content Download Request
#define search_pdu	'S'	//Search for content and the associated content server
#define dereg_pdu	'T'	//Content De-Registration
#define data_pdu	'C'	//Content Data
#define list_pdu	'O'	//List of On-Line Registered Content
#define ack_pdu 	'A'	//Acknowledgement
#define err_pdu 	'E'	//Error

struct pdu {
	char type;
	char data[100];
};