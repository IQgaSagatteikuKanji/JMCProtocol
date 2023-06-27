#pragma once
#include <stdint.h>

// I haven't found the library to use yet, so Ill use this module to add that library easier

// Contracts:
// - Allocates memory for destination and transfers ownership

struct archiver{

};

struct string{
    uint32_t length;
    char *str;
};

void archive_string(struct archiver *archiver, struct string *src, struct string *dest);
void archive_string(struct achiver *archiver, char *src, uint32_t length, struct string *dest);

void dearchive_string(struct achiver *archiver, struct string *src, struct string *dest);
void dearchive_string(struct achiver *archiver, char *src, uint32_t length, struct string *dest);

void string_init(struct string *str);
void string_destroy(struct string *str);

void archiver_init(struct archiver *archiver);
void archiver_destroy(struct archiver *archiver);