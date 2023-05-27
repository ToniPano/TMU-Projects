#define DATA_PDU		'D'
#define FINAL_PDU		'F'
#define ERROR_PDU		'E'
#define FILENAME_PDU	'C'

struct pdu {
	char type;
	char data[100];
};