#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//Parameters for AES-128
#define Nr 10
#define Nk 4
#define Nb 4

typedef struct{

	uint8_t state[4][4];

} T_STATE;

//The main AES functions
uint8_t SBox(uint8_t input);
void subBytes(T_STATE* input);
void shiftRows(T_STATE* input_state);
void mixColumns(T_STATE* input_state);
void addRoundKey(T_STATE* input_state, T_STATE* round_key);
T_STATE** keyExpansion(uint8_t* key);
void cipher(uint8_t* key, T_STATE* input);

//Helper function to multiply by 2 in GF(256)
uint8_t xtime(uint8_t input);

//Helper functions for the key expansion schedule
void subWord(uint8_t* input_word);
void rotWord(uint8_t* input_word);
