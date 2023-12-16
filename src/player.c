#include "../include/player.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/colors.h"
#include "../include/map.h"

player_t *init_player(void) {
    player_t *player = calloc(1, sizeof(player_t));

    while (true) {
        player->x = rand() % MAP_HEIGHT;
        player->y = rand() % MAP_WIDTH;

        if (get_mapch(player->y, player->x) == '.')
            break;
    }

    return player;
}

void draw_player(player_t *player) {
    attron(COLOR_PAIR(PLAYER_COLOR_PAIR));
    mvaddch(player->y, player->x, '@');
    attroff(COLOR_PAIR(PLAYER_COLOR_PAIR));
}

void clear_player(player_t *player) {
    u16 c = get_mapch(player->y, player->x);
    mvaddch(player->y, player->x, c);
}

/* clang-format off */
void move_player(player_t *player, int c) {
    i16 newx = player->x;
    i16 newy = player->y;

    if (c == 'h') newx--;
    else if (c == 'l') newx++;
    else if (c == 'k') newy--;
    else if (c == 'j') newy++;
    else if (c == 'y') { newy--; newx--; }
    else if (c == 'u') { newy--; newx++; }
    else if (c == 'b') { newy++; newx--; }
    else if (c == 'n') { newy++; newx++; }

    int ch = get_mapch(newy, newx);
    if (ch == '.' || ch == '=' || ch == '#') {
        player->y = newy;
        player->x = newx;
    }

    reveal_partial_map(player->y, player->x);
}
/* clang-format on */
