#include "../include/player.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/colors.h"
#include "../include/debug.h"
#include "../include/helper.h"
#include "../include/items.h"
#include "../include/map.h"

player_t *init_player(void) {
    player_t *player = malloc(sizeof(player_t));

    *player = (player_t){0};

    player->health = 10;
    player->damage = 2;
    player->speed = 1;

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

void move_player(player_t *player, int c) {
    int newx = player->x;
    int newy = player->y;
    bool pickup = false;

    /* clang-format off */
    switch (c) {
        case 'h': newx--; break;
        case 'l': newx++; break;
        case 'k': newy--; break;
        case 'j': newy++; break;
        case 'y': newy--; newx--; break;
        case 'u': newy--; newx++; break;
        case 'b': newy++; newx--; break;
        case 'n': newy++; newx++; break;
        case ',': pickup = true; break;
    }
    /* clang-format on */

    /* only handle movement if we moved */
    int ch = get_mapch(newy, newx);
    if (ch == '.' || ch == '=' || ch == '#') {
        player->y = newy;
        player->x = newx;
    }

    reveal_partial_map(player->y, player->x, PLAYER_FOV);

    if (pickup) {
        item_t *item = find_item(player->y, player->x);

        if (item->symbol == '$') {
            player->money += item->amount;
        }

        remove_item(item);
    }
}

void draw_player_stats(player_t player) {
    attrset(COLOR_PAIR(DEFAULT_COLOR_PAIR) | A_BOLD);
    move(MAP_HEIGHT + 1, 1);
    /* increases readability instead of putting all on one line */
    printw("$: %d, ", player.money);
    printw("HP: %d, ", player.health);
    printw("DMG: %d", player.damage);
}
