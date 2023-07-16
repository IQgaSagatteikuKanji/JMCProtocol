#pragma once

#include <ncurses.h>
#include <stdbool.h>

#define COMMAND_LENGTH 1022
#define LINE_STARTER ">"
#define LINE_STARTER_LENGTH sizeof(LINE_STARTER) - 1
#define LENGTH_OF_COMMAND_LINE_BUFFER (COMMAND_LENGTH + LINE_STARTER_LENGTH)

bool serving;

struct client_console{
    uint32_t height, width;
    WINDOW *border;
    char buffer[LENGTH_OF_COMMAND_LINE_BUFFER + 1];
    uint16_t bufpos;
};

void client_console_init(struct client_console *ccon, uint32_t width, uint32_t height);
void client_console_destroy(struct client_console *ccon);

void ccon_manage_input(struct client_console *ccon);