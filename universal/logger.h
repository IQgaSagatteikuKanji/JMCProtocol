#pragma once
#include <stdint.h>

// Contracts:
// - Expects no collisions of file desriptors
// - Strings are '\0' terminated
// - Memory allocated, ownership transfered
// - logger_builder doesn't have defined lifecycle to accomodate both string literals and string variables

struct logger;

struct logger_builder{
    char *filename;
    char *path;
};

void logger_init(struct logger *logger, struct logger_builder * builder);
void logger_destroy(struct logger *logger);

void log(struct logger *logger, char *str);
void log(struct logger *logger, char *str, uint32_t length);
