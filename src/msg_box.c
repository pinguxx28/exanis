#include "../include/msg_box.h"

#include "../include/debug.h"
#include "../include/colors.h"
#include "../include/map.h"
#include <ncurses.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

int i = 0;
char *msg_box = NULL;
char *new_msg_box = NULL;

void load_msg_box(char *msg, ...) {
    va_list arg;
    va_start(arg, msg);

	if (strlen(msg) >= 80) {
		NC_ABORT("msg too long\n[%s]\naborting\n", msg);
	}
    char buf[81];
    vsprintf(buf, msg, arg);

    if (i + strlen(buf) >= 80) strcat(new_msg_box, buf);
    else strcat(msg_box, buf);

    i += strlen(buf);

    va_end(arg);
}

void init_msg_box(void) {
    msg_box = malloc(81);
    new_msg_box = malloc(81);
}

void clear_msg_box(void) {
    i = 0;

    if (msg_box == NULL) return;
    msg_box[0] = '\0';

    if (new_msg_box == NULL) return;
    new_msg_box[0] = '\0';
}

void draw_msg_box(void) {
    if (msg_box == NULL) return;

    attrset(COLOR_PAIR(DEFAULT_COLOR_PAIR) | A_BOLD);
    mvprintw(MAP_HEIGHT + 1, 0, "%-80s", msg_box);

    if (new_msg_box == NULL || new_msg_box[0] == '\0') return;

    attrset(COLOR_PAIR(INVERSE_DEFAULT_COLOR_PAIR) | A_BOLD);
    printw("MORE");

    /* wait for space */
    while (getch() != ' ')
        ;

    attrset(COLOR_PAIR(DEFAULT_COLOR_PAIR) | A_BOLD);
    mvprintw(MAP_HEIGHT + 1, 0, "%-84s", new_msg_box); /* 84 to delete MORE */
}
