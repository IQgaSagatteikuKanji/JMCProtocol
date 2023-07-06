#include "logger.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

void logger_init(struct logger *logger, struct logger_builder *builder){
    assert(logger != NULL);
    assert(builder != NULL);
    assert(builder->filename != NULL);

    logger->logfile = fopen(builder->filename, "w");
    memset(logger->buffer, 0, LOGGER_BUFFER_SIZE);
    logger->offset = 0u;
    logger->zero_terminator_for_buffer = 0;
}


void logger_flush(struct logger *logger){
    fputs(logger->buffer, logger->logfile);
    memset(logger->buffer, 0, LOGGER_BUFFER_SIZE);
    
    logger->offset = 0u;
}

void logger_destroy(struct logger *logger){
    assert(logger != NULL);

    if(logger->offset > 0){
        logger_flush(logger);
    }

    fclose(logger->logfile);
    logger->logfile = NULL;
}

char *get_0_terminated_str_from(char *str, uint32_t length){
    char * retval = calloc(1, length + 1);
    memcpy(retval, str, length);
    return retval;
}

void log_fixed_0_term_str(struct logger *logger, char *str, uint32_t length){
    if(length > LOGGER_BUFFER_SIZE){
        logger_flush(logger);
        fputs(str, logger->logfile);
    } else{
        if(length > LOGGER_BUFFER_SIZE - logger->offset){
            logger_flush(logger);
        }
        strncpy(logger->buffer + logger->offset, str, length);
        logger->offset = logger->offset + length;
    }
}

void log_0_terminated_str(struct logger *logger, char *str){
    assert(logger != NULL);
    assert(str != NULL);

    uint16_t length = strlen(str);
    log_fixed_0_term_str(logger, str, length);
}

void log_fixed_length_str(struct logger *logger, char *str, uint32_t length){
    assert(logger != NULL);
    assert(str != NULL);

    char *terminated_str = get_0_terminated_str_from(str, length);
    log_fixed_0_term_str(logger, str, length);
    free(terminated_str);
}
