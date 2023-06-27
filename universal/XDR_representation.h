#pragma once
#include <stdint.h>
#include <stdbool.h>

// Mostly follows the standard XDR RFC 4506(STD 67)
// But adds integers of sizes smaller than 32bit (16bit, 8bit) and bigger (64bit)
// and encodes booleans as the signed 8bit integer 

// The flag to toggle if residual zero bytes should be added to the opaque data
// I myself do not use it, but if I need it later it is easy to toggle it on
#define RESIDUAL_ZEROS_AT_END_OF_OPAQUE false 


// Contracts:
// - Exception to the contract is translate_XDR_to_variable_length_opaque because of its nature 
//      (contract for it is written at a definition of the function)
// - Expects all parameters to be allocated outside
// - Expects char strings to be long enough for the types(check return value of sizeof(desired type))
// - Doesn't add line end delimeters at the end
// - Doesn't allocate new or free existing memory
// - Returns how much bytes were read/written

//boolean
uint32_t translate_boolean_to_XDR(bool *boolean, char *to);
uint32_t translate_XDR_to_boolean(char *from, bool *boolean);

//floating point numbers
uint32_t translate_double_to_XDR(double *number, char *to);
uint32_t translate_XDR_to_double(char *from, double *number);

uint32_t translate_float_to_XDR(float *number, char *to);
uint32_t translate_XDR_to_float(char *from, float *number);

//integers
uint32_t translate_uint64_to_XDR(uint64_t *number, char *to);
uint32_t translate_XDR_to_uint64(char *from, uint64_t *number);

uint32_t translate_int64_to_XDR(int64_t *number, char *to);
uint32_t translate_XDR_to_int64(char *from, int64_t *number);

uint32_t translate_uint32_to_XDR(uint32_t *number, char *to);
uint32_t translate_XDR_to_uint32(char *from, uint32_t *number);

uint32_t translate_int32_to_XDR(int32_t *number, char *to);
uint32_t translate_XDR_to_int32(char *from, int32_t *number);

uint32_t translate_uint16_to_XDR(uint16_t *number, char *to);
uint32_t translate_XDR_to_uint16(char *from, uint16_t *number);

uint32_t translate_int16_to_XDR(int16_t *number, char *to);
uint32_t translate_XDR_to_int16(char *from, int16_t *number);

uint32_t translate_uint8_to_XDR(uint8_t *number, char *to);
uint32_t translate_XDR_to_uint8(char *from, uint8_t *number);

uint32_t translate_int8_to_XDR(int8_t *number, char *to);
uint32_t translate_XDR_to_int8(char *from, int8_t *number);

// opaque data
uint32_t translate_fixed_length_opaque_to_XDR(char *opaque, uint32_t length, char *to);
uint32_t translate_XDR_to_fixed_length_opaque(char *from, uint32_t length, char *opaque);



uint32_t translate_variable_length_opaque_to_XDR(char *opaque, uint32_t length, char *to); 

// Allocates memory and transfers ownership
// Expects a pointer to a NULL initialised pointer
// Returns offset in encoded data
// Length parameter is filled with the length of the read opaque data
// !!! WILL NOT ADD '\0' IF ITS NOT IN "FROM" !!!
uint32_t translate_XDR_to_variable_length_opaque(char *from, char **opaque, uint32_t *length);