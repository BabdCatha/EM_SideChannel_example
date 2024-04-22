typedef struct{

	char state[4][4];

} T_STATE;

//The main AES functions
char subBytes(char input);
void shiftRows(T_STATE* input_state);
void mixColumns(T_STATE* input_state);
void addRoundKey(T_STATE* input_state, T_STATE* round_key);
T_STATE* keyExpansion(char* key);

//Helper function to multiply by 2 in GF(256)
char xtime(char input);

//Helper functions for the key expansion schedule
void subWord(char* input_word);
void rotWord(char* input_word);
