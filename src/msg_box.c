#include "../include/msg_box.h"

#include "../include/map.h"
#include <ncurses.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

char *msg_box = NULL;

void load_msg_box(char *msg, ...) {
    va_list arg;

    va_start(arg, msg);
    char buf[81];
    vsprintf(buf, msg, arg);
    strcat(msg_box, buf);
    va_end(arg);
}

void init_msg_box(void) {
    msg_box = malloc(80);
}

void clear_msg_box(void) {
    if (msg_box == NULL) return;
    msg_box[0] = '\0';
}

void display_msg_box(void) {
    if (msg_box != NULL) {
        mvprintw(MAP_HEIGHT + 2, 0, "%-80s", msg_box);
    }
}
