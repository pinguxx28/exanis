#include "../include/player.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/colors.h"
#include "../include/debug.h"
#include "../include/helper.h"
#include "../include/items.h"
#include "../include/map.h"
#include "../include/monsters.h"
#include "../include/msg_box.h"

player_t *init_player(void) {
    player_t *player = malloc(sizeof(player_t));

    *player = (player_t){0};

    player->health = 15;
    player->damage = 2;
    player->speed = 1;

    do {
        player->y = random_i(0, MAP_HEIGHT);
        player->x = random_i(0, MAP_WIDTH);
    } while (get_mapch(player->y, player->x) != '.');

    return player;
}

void draw_player(player_t player) {
    attrset(COLOR_PAIR(PLAYER_COLOR_PAIR) | A_BOLD);
    mvaddch(player.y, player.x, '@');
}

void clear_player(player_t player) {
    int c = get_mapch(player.y, player.x);
    mvaddch(player.y, player.x, c);
}

void move_player(player_t *player, int c) {
    int newy = player->y;
    int newx = player->x;
    bool pickup = false;
    bool decend = false;

    /* clang-format off */
    switch (c) {
        case 'k': newy--; break;
        case 'j': newy++; break;
        case 'h': newx--; break;
        case 'l': newx++; break;
        case 'y': newy--; newx--; break;
        case 'u': newy--; newx++; break;
        case 'b': newy++; newx--; break;
        case 'n': newy++; newx++; break;
        case ',': pickup = true; break;
        case '>': decend = true; break;
    }
    /* clang-format on */

    /* only handle movement if we moved */
    int ch = get_mapch(newy, newx);
    monster_t *monster = find_monster(newy, newx);

    if (monster != NULL) {
        int old_health = monster->health;
        monster->health -= player->damage;
        if (monster->health <= 0) monster->health = 0;
        load_msg_box("You hit %s HP: %d->%d! ", monster->name, old_health,
                     monster->health);
    } else if (ch != '-' && ch != '|' && ch != ' ') {
        player->y = newy;
        player->x = newx;
    }

    reveal_partial_map(player->y, player->x, PLAYER_FOV);

    if (pickup) {
        item_t *item = find_item(player->y, player->x);

        if (item == NULL) {
            load_msg_box("Nothing on the ground\n ");
            return;
        }

        if (item->symbol == '$') {
            player->money += item->amount;
            load_msg_box("Cha-ching +$%d", item->amount);
        }

        remove_item(item);
    }

    if (decend) {
        if (get_mapch(player->y, player->x) != '>') {
            load_msg_box("Can't decend here. ");
        }

        /* TODO: make this better */
        endwin();
        printf("You decended with $%d\n", player->money);
        exit(0);
    }
}

void draw_player_stats(player_t player) {
    attrset(COLOR_PAIR(DEFAULT_COLOR_PAIR) | A_BOLD);
    move(MAP_HEIGHT, 0);

    char stat_line[81];
    snprintf(stat_line, 80, "$: %d, HP: %d, DMG: %d", player.money,
             player.health, player.damage);
    printw("%-80s", stat_line);
}
