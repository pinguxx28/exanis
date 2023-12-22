#include "../include/colors.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

void ensure_colors(void) {
    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal doesn't suppport color\n");
        exit(1);
    }
}

void init_color_pairs(void) {
    init_color(COLOR_YELLOW, 1000, 1000, 0);

    init_pair(DEFAULT_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(PLAYER_COLOR_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(MONEY_COLOR_PAIR, COLOR_YELLOW, COLOR_BLACK);
}
