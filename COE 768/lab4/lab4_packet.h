#define DATA_PACKET 'D'
#define ERROR_PACKET 'E'

struct packet{
    char type;
    char data[99];
};  //need semicolon at end of struct
