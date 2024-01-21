#include "map.h"
#include "items.h"
#include "colors.h"
#include "player.h"
#include "msg_box.h"
#include "monsters.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>

int main(int argc, char *argv[]) {
	bool godmode = false;
	if (argc == 2 && !strcmp(argv[1], "godmode")) {
		godmode = true;
	}

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

    player_t *player = init_player(godmode);

    int c = ' ';
	do {
        clear_player(*player);
        clear_msg_box();

        /* updating */
        update_player(player, c);
        update_monsters(player->y, player->x, &player->health, player->protection);

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
