#include "client_console.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "client_command_handler.h"

#define DEFAULT_CONSOLE_HEIGHT 5

void client_console_init(struct client_console *ccon, uint32_t width, uint32_t height){
    assert(ccon != NULL);
    serving = true;

    ccon->height = DEFAULT_CONSOLE_HEIGHT;
    ccon->width = width;
    ccon->border = newwin(ccon->height + 1, ccon->width, height - ccon->height - 1, 0);
    ccon->bufpos = 0;
    memset(ccon->buffer, 0, LENGTH_OF_COMMAND_LINE_BUFFER + 1);
}

void client_console_destroy(struct client_console *ccon){
    assert(ccon != NULL);
}


void ccon_print_command_line_buffer(struct client_console *ccon){
    mvwprintw(ccon->border, 1, 0, ccon->buffer);
    
}

void ccon_draw_user_console(struct client_console *ccon){
    wclear(ccon->border);

    wborder(ccon->border, ' ', ' ', ACS_HLINE, ' ', ACS_HLINE, ACS_HLINE , ' ', ' ');
    ccon_print_command_line_buffer(ccon);

    wrefresh(ccon->border);
}

void ccon_clear_user_console(struct client_console *ccon){
    memset(ccon->buffer, 0, LENGTH_OF_COMMAND_LINE_BUFFER);
    ccon->buffer[LENGTH_OF_COMMAND_LINE_BUFFER - 1] = 0;
    strncpy(ccon->buffer, LINE_STARTER, LINE_STARTER_LENGTH);
    ccon->bufpos = 0;

    ccon_draw_user_console(ccon);
}


void ccon_manage_input(struct client_console *ccon){
    assert(ccon != NULL);

    ccon_clear_user_console(ccon);
    wmove(ccon->border, 1, LINE_STARTER_LENGTH);

    while(serving){
        int c = getch();
        switch(c){
            case 127:
            case KEY_DC:
            case KEY_BACKSPACE:
                if(ccon->bufpos != 0){
                    ccon->bufpos--;
                }
                ccon->buffer[LINE_STARTER_LENGTH + ccon->bufpos] = 0;
                ccon_draw_user_console(ccon);

                break;
            case '\n':
                command_handler(ccon->buffer + LINE_STARTER_LENGTH);
                refresh();
                ccon_clear_user_console(ccon);
                break;

            default:
                if(ccon->bufpos + LINE_STARTER_LENGTH < LENGTH_OF_COMMAND_LINE_BUFFER){
                    ccon->buffer[LINE_STARTER_LENGTH + ccon->bufpos++] = c;
                    ccon_draw_user_console(ccon);
                }
                break;
        }
    }
}