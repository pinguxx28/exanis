#include "../include/player.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/colors.h"
#include "../include/helper.h"
#include "../include/map.h"

player_t *init_player(void) {
    player_t *player = malloc(sizeof(player_t));

    do {
        player->x = random_i(0, MAP_WIDTH);
        player->y = random_i(0, MAP_HEIGHT);
    } while (get_mapch(player->y, player->x) != '.');

    return player;
}

void draw_player(player_t *player) {
    attrset(COLOR_PAIR(PLAYER_COLOR_PAIR) | A_BOLD);
    mvaddch(player->y, player->x, '@');
}

void clear_player(player_t *player) {
    int c = get_mapch(player->y, player->x);
    mvaddch(player->y, player->x, c);
}

/* clang-format off */
void move_player(player_t *player, int c) {
    int newx = player->x;
    int newy = player->y;

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

    reveal_partial_map(player->y, player->x, PLAYER_FOV);
}
/* clang-format on */
