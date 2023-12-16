#include "../include/colors.h"
#include "../include/defs.h"
#include "../include/player.h"
#include <ncurses.h>

int main() {
    u8 c = 0;

    initscr();
    noecho();
    raw();
    curs_set(0);

    ensure_colors();
    start_color();
    init_color_pairs();

    player_t *player = init_player();

    do {
        clear_player(player);

        /* updating */
        if (c) {
            move_player(player, c);
        }

        /* drawing */
        draw_player(player);
        refresh();
    } while ((c = getch()) != 'q');

    endwin();

    return 0;
}
