#pragma once
#include <stdint.h>
#include <stdio.h>


// Contracts:
// - Expects no collisions of file descriptors
// - Strings are '\0' terminated
// - Memory allocated, ownership transfered
// - logger_builder doesn't have defined lifecycle to accomodate both string literals and string variables

#define LOGGER_BUFFER_SIZE 512

struct logger{
    FILE *logfile;
    uint8_t offset;
    char buffer[LOGGER_BUFFER_SIZE];
    char zero_terminator_for_buffer;
};



struct logger_builder{
    char *filename;
};

void logger_init(struct logger *logger, struct logger_builder *builder);
void logger_destroy(struct logger *logger);

void log_0_terminated_str(struct logger *logger, char *str);
void log_fixed_length_str(struct logger *logger, char *str, uint32_t length);
