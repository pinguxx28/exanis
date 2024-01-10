#include "msg_box.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "map.h"
#include "debug.h"
#include "colors.h"

#define MAXSTRLEN 80

int i = 0;
char *msg_box = NULL;
char *new_msg_box = NULL;

void load_msg_box(char *msg, ...) {
	/* not the most beautiful code */
	/* please improve */

	if (strlen(msg) > MAXSTRLEN) {
		NC_ABORT("msg too long\n[%s]\naborting\n", msg);
	}

    va_list arg;
    va_start(arg, msg);

    char buffer[81];
    vsprintf(buffer, msg, arg);

    if (i + strlen(buffer) > MAXSTRLEN) {
		strcat(new_msg_box, buffer);
	} else {
		strcat(msg_box, buffer);
	}

    i += strlen(buffer);

    va_end(arg);
}

void init_msg_box(void) {
    msg_box     = malloc(MAXSTRLEN + 1);
    new_msg_box = malloc(MAXSTRLEN + 1);
    msg_box[0]     = '\0';
    new_msg_box[0] = '\0';
}

void clear_msg_box(void) {
    i = 0;
    msg_box[0] = '\0';
    new_msg_box[0] = '\0';
}

void draw_msg_box(void) {
	/* same case as for load_msg_box */
    attrset(COLOR_PAIR(DEFAULT_COLOR_PAIR) | A_BOLD);
    mvprintw(MAP_HEIGHT + 1, 0, "%-80s", msg_box);

    if (new_msg_box[0] == '\0') return;

    attrset(COLOR_PAIR(INVERSE_DEFAULT_COLOR_PAIR) | A_BOLD);
    printw("MORE");

    /* wait for space */
    while (getch() != ' ') ;

    attrset(COLOR_PAIR(DEFAULT_COLOR_PAIR) | A_BOLD);
    mvprintw(MAP_HEIGHT + 1, 0, "%-84s", new_msg_box); /* 84 to delete MORE */
}
