#include "../include/colors.h"
#include "../include/debug.h"
#include "../include/items.h"
#include "../include/map.h"
#include "../include/player.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(0));
    open_debug_file("debug.txt");

    int c = 'k';

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
        draw_items(player->x, player->y, 10);
        draw_player(player);
        refresh();
    } while ((c = getch()) != 'q');

    endwin();
    free(player);

    return 0;
}
