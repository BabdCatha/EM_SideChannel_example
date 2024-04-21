typedef struct{

	char state[4][4];

} T_STATE;

//The main AES functions
char subBytes(char input);
void shiftRows(T_STATE* input_state);
void mixColumns(T_STATE* input_state);

//Helper function to multiply by 2 in GF(256)
char xtime(char input);
