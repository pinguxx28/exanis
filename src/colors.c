#include "../include/colors.h"

#include "../include/debug.h"

void ensure_colors(void) {
    if (has_colors() == FALSE) {
        NC_ABORT("terminal doesn't support colors\n");
    }
}

void init_color_pairs(void) {
    if (can_change_color() == TRUE) {
        init_color(COLOR_WHITE, 500, 500, 500);
        init_color(COLOR_BLACK, 0, 0, 0);

        const int BRIGHT = 8;
        init_color(COLOR_WHITE + BRIGHT, 1000, 1000, 1000);
        init_color(COLOR_CYAN + BRIGHT, 0, 808, 819);
        init_color(COLOR_YELLOW + BRIGHT, 1000, 917, 0);
        init_color(COLOR_RED + BRIGHT, 1000, 0, 0);
        init_color(COLOR_GREEN + BRIGHT, 0, 500, 0);
    }

    init_pair(DEFAULT_COLOR_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(INVERSE_DEFAULT_COLOR_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(PLAYER_COLOR_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(MONEY_COLOR_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(MONSTERS_COLOR_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(ORC_COLOR_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(WEAPON_COLOR_PAIR, COLOR_MAGENTA, COLOR_BLACK);
}
