#include "../include/colors.h"
#include "../include/items.h"
#include "../include/map.h"
#include "../include/monsters.h"
#include "../include/msg_box.h"
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
    init_monsters();
    init_msg_box();

    player_t *player = init_player();

    do {
        clear_player(player);
        clear_msg_box();

        /* updating */
        move_player(player, c);
        update_monsters(player->y, player->x, &player->health);

        /* drawing */
        print_map();
        draw_items(player->y, player->x, PLAYER_FOV);
        draw_monsters(player->y, player->x, PLAYER_FOV);
        draw_player(player);
        draw_player_stats(*player);
        display_msg_box();
        refresh();
    } while ((c = getch()) != 'q');

    endwin();
    free(player);

    return 0;
}
