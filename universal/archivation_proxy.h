#pragma once
#include <stdint.h>

// I haven't found the library to use yet, so Ill use this module to add that library easier

// Contracts:
// - Allocates memory for destination and transfers ownership

#define MAX_SIZE_ARCHIVED_PACKET 1024

void archive_string(char *src, uint32_t length, char *dest, uint32_t *output_length);
void dearchive_string(char *src, uint32_t length, char *dest, uint32_t *output_length);
