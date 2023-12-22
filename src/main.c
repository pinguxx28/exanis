#include "../include/colors.h"
#include "../include/items.h"
#include "../include/map.h"
#include "../include/player.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(0));

    int c = 'h';

    initscr();
    noecho();
    raw();
    curs_set(0);

    ensure_colors();
    start_color();
    init_color_pairs();

    generate_map();
    create_items();

    player_t *player = init_player();

    do {
        clear_player(player);

        /* updating */
        move_player(player, c);

        /* drawing */
        print_map();
        draw_items(player->x, player->y, PLAYER_FOV);
        draw_player(player);
        draw_player_stats(*player);
        refresh();
    } while ((c = getch()) != 'q');

    endwin();
    free(player);

    return 0;
}
