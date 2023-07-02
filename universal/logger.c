#include "logger.h"

#include <assert.h>
#include <string.h>

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
    fwrite(logger->buffer, 1, logger->offset, logger->logfile);
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

void log_0_terminated_str(struct logger *logger, char *str){
    assert(logger != NULL);
    assert(str != NULL);

    uint16_t length = strlen(str);
    log_fixed_length_str(logger, str, length);
}

void log_fixed_length_str(struct logger *logger, char *str, uint32_t length){
    if(length > LOGGER_BUFFER_SIZE){
        logger_flush(logger);
        fwrite(str, 1, length, logger->logfile);
    } else{
        if(length > LOGGER_BUFFER_SIZE - logger->offset){
            logger_flush(logger);
        }
        strncpy(logger->buffer + logger->offset, str, length);
        logger->offset = logger->offset + length;
    }
}
