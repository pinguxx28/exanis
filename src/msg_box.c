#include "../include/msg_box.h"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

char *msg_box = NULL;

void load_msg_box(char *msg) {
    strcat(msg_box, msg);
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
        mvprintw(0, 0, "%-80s", msg_box);
    }
}
