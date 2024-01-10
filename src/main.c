#include "colors.h"
#include "items.h"
#include "map.h"
#include "monsters.h"
#include "msg_box.h"
#include "player.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

int main() {
	/* non-ncurses setup */
    srand(time(0));

	/* ncurses setup */
    initscr();
    noecho();
    raw();
    curs_set(0);

	/* colors */
    ensure_colors();
    start_color();
    init_color_pairs();

	/* exanis features */
    create_map();
    create_items();
    create_monsters();
    init_msg_box();

    player_t *player = init_player();

    int c = ' ';
	do {
        clear_player(*player);
        clear_msg_box();

        /* updating */
        update_player(player, c);
        update_monsters(player->y, player->x, &player->health);

        /* map drawing */
        draw_map();
        draw_items(player->y, player->x, PLAYER_FOV);
        draw_monsters(player->y, player->x, PLAYER_FOV);
        draw_player(*player);

		/* line drawing */
        draw_player_stats(*player);
		draw_msg_box();

        refresh();
    } while ((c = getch()) != 'q');

    endwin();
    free(player);

    return 0;
}
