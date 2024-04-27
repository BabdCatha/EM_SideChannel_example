#include "AES.h"

uint8_t subBytes(uint8_t input){

	uint8_t sbox[256] = {
	    // 0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
	    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,  // 0
	    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,  // 1
	    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,  // 2
	    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,  // 3
	    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,  // 4
	    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,  // 5
	    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,  // 6
	    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,  // 7
	    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,  // 8
	    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,  // 9
	    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,  // A
	    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,  // B
	    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,  // C
	    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,  // D
	    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,  // E
	    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}; // F

	return sbox[input];

}

void shiftRows(T_STATE* input_state){

	//A temporary array to store a copy of the line being worked on
	uint8_t* temp_array = malloc(4*sizeof(char));

	//Loop variables
	uint8_t i = 1;
	uint8_t j = 0;

	//Looping over lines 1,2,3 of the state
	//Line 0 stays unchanged by the shiftRows operation
	for(i = 1; i < 4; i++){

		//Copying the line being worked on to the temporary array
		memcpy(temp_array, input_state->state[i], 4*sizeof(char));

		//Looping over the line, shifting the bytes as required
		for(j = 0; j < 4; j++){
			input_state->state[i][j] = temp_array[(i+j) % 4];
		}

	}

	//Releasing the temporary array
	free(temp_array);

}

void mixColumns(T_STATE* input_state){

	//A temporary array to store a copy of the column being worked on
	uint8_t* temp_array = malloc(4*sizeof(char));

	//Loop variables
	uint8_t i = 0; //Line
	uint8_t j = 0; //Column

	//Iterating over the columns
	for(j = 0; j < 4; i++){

		//Copying the column into temporary memory
		for(i = 0; i < 4; i++){
			temp_array[i] = input_state->state[i][j];
		}

		//Matrix multiplication on the column, using the xtime helper function
		//See part 5.1.3 of FIPS-197 for detailed explanation
		input_state->state[0][j] = xtime(temp_array[0]) ^ (xtime(temp_array[1]) ^ temp_array[1]) ^ temp_array[2] ^ temp_array[3];
		input_state->state[1][j] = temp_array[0] ^ xtime(temp_array[1]) ^ (xtime(temp_array[2]) ^ temp_array[2]) ^ temp_array[3];
		input_state->state[2][j] = temp_array[0] ^ temp_array[1] ^ xtime(temp_array[2]) ^ (xtime(temp_array[3]) ^ temp_array[3]);
		input_state->state[3][j] = (xtime(temp_array[0]) ^ temp_array[0]) ^ temp_array[1] ^ temp_array[2] ^ xtime(temp_array[3]);

	}

	//Freeing the temporary array
	free(temp_array);

}

//XOR the current state with the round key, byte by byte
void addRoundKey(T_STATE* input_state, T_STATE* round_key){

	//Loop variables
	uint8_t i = 0; //Line
	uint8_t j = 0; //Column

	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			input_state->state[i][j] = input_state->state[i][j] ^ round_key->state[i][j];
		}
	}

}

//See section 5.2 of FIPS-197 for technical explanations
//For AES-128, Nr = 10; Nb = 4 and Nk = 4.
T_STATE** keyExpansion(uint8_t* key){

	//Output variable
	T_STATE** output_array;
	output_array = malloc((Nr + 1)*sizeof(T_STATE*));

	//Loop variable
	uint8_t i = 0;
	uint8_t j = 0;

	for(i = 0; i <= Nr; i++){
	    output_array[i] = malloc(sizeof(T_STATE));
	}

	//Temp variable to hold the intermediate steps
	uint8_t** w = malloc((4 * (Nr + 1)) * sizeof(uint8_t*));
	for(i = 0; i < (4 * (Nr + 1)); i++){
	    w[i] = malloc(4*sizeof(uint8_t));
	}

	//Key expansion constants
	uint8_t Rcon[10][4] = {
			{0x01, 0x00, 0x00, 0x00},
			{0x02, 0x00, 0x00, 0x00},
			{0x04, 0x00, 0x00, 0x00},
			{0x08, 0x00, 0x00, 0x00},
			{0x10, 0x00, 0x00, 0x00},
			{0x20, 0x00, 0x00, 0x00},
			{0x40, 0x00, 0x00, 0x00},
			{0x80, 0x00, 0x00, 0x00},
			{0x1b, 0x00, 0x00, 0x00},
			{0x36, 0x00, 0x00, 0x00}
	};

	i = 0;
	while(i < Nk){
		w[i][0] = key[4*i];
		w[i][1] = key[4*i+1];
		w[i][2] = key[4*i+2];
		w[i][3] = key[4*i+3];
		i++;
	}

	//Another temporary variable
	uint8_t* temp = malloc(4*sizeof(uint8_t));

	while(i <= (4 * Nr) + 3){

		memcpy(temp, w[i-1], 4*sizeof(uint8_t));

		if(i% Nk == 0){
			rotWord(temp);
			subWord(temp);
			temp[0] = temp[0] ^ Rcon[(i/Nk) - 1][0];
			temp[1] = temp[1] ^ Rcon[(i/Nk) - 1][1];
			temp[2] = temp[2] ^ Rcon[(i/Nk) - 1][2];
			temp[3] = temp[3] ^ Rcon[(i/Nk) - 1][3];
		}

		w[i][0] = w[i - Nk][0] ^ temp[0];
		w[i][1] = w[i - Nk][1] ^ temp[1];
		w[i][2] = w[i - Nk][2] ^ temp[2];
		w[i][3] = w[i - Nk][3] ^ temp[3];

		i++;

	}

	//At this point, the columns of the round keys have been computed and stored in w
	//We transform them back into states

	for(i = 0; i <= Nr; i++){
		for(j = 0; j < 4; j++){
			output_array[i]->state[0][j] = w[4*i+j][0];
			output_array[i]->state[1][j] = w[4*i+j][1];
			output_array[i]->state[2][j] = w[4*i+j][2];
			output_array[i]->state[3][j] = w[4*i+j][3];
		}
	}

	return output_array;

}

uint8_t xtime(uint8_t input){

	//See FIPS-197 for explanations
	uint8_t result = input << 1;
	result = result ^ 0x1b;

	return result;

}

void subWord(uint8_t* input_word){

	//Loop variable
	uint8_t i = 0;

	for(i = 0; i < 4; i++){
		input_word[i] = subBytes(input_word[i]);
	}

}

void rotWord(uint8_t* input_word){

	//temp variable
	uint8_t temp = input_word[0];
	input_word[0] = input_word[1];
	input_word[1] = input_word[2];
	input_word[2] = input_word[3];
	input_word[3] = temp;

}
