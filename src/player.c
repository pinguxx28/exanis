#include "../include/player.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

    player->weapon1 = make_weapon(FIST);
    player->weapon2 = make_weapon(FIST);

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

static void update_player_weapon(player_t *player) {
	player->damage = player->weapon1.damage + player->weapon2.damage;
}

static void move_player(player_t *player, int c) {
    int newy = player->y;
    int newx = player->x;

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
    }
    /* clang-format on */

    /* only handle movement if we moved */
    if (newx == player->x && newy == player->y) return;

    int ch = get_mapch(newy, newx);
    monster_t *monster = find_monster(newy, newx);

    if (monster != NULL) {
        int old_health = monster->health;
        monster->health -= player->damage;
        monster->health = max(monster->health, 0);

		if (player->weapon1.type == player->weapon2.type) {
			load_msg_box(
				"You hit %s with %ss, their HP: %d->%d! ",
				monster->name, player->weapon1.name, old_health, monster->health);
		} else {
			load_msg_box(
				"You hit %s with a %s and a %s, their HP: %d->%d! ",
				monster->name, player->weapon1.name, player->weapon2.name, old_health, monster->health);
		}
    } else if (ch != '-' && ch != '|' && ch != ' ') {
        player->y = newy;
        player->x = newx;
    }
}

static void pickup_player(player_t *player) {
    item_t *item = find_item(player->y, player->x);

    if (item == NULL) {
        load_msg_box("Nothing on the ground. ");
        return;
    }

    switch (item->type) {
        case MONEY:
            player->money += item->amount;
            load_msg_box("Cha-ching +$%d. ", item->amount);
            break;
        case WEAPON:
            if (player->weapon1.type == FIST) {
                player->weapon1 = item->weapon;
            } else if (player->weapon2.type == FIST) {
                player->weapon2 = item->weapon;
            }

			update_player_weapon(player);

            load_msg_box("Picked up a %s. ", item->weapon.name);
			load_msg_box("Equipped a %s. ", item->weapon.name);
            break;
        default:
            load_msg_box("Picked up %d %s. ", item->amount, item->name);
            break;
    }

    remove_item(item);
}

static void decend_player(player_t *player) {
    if (get_mapch(player->y, player->x) != '>') {
        load_msg_box("Can't decend here. ");
    }

    /* TODO: make this better */
    endwin();
    printf("You decended with $%d\n", player->money);
    exit(0);
}

void update_player(player_t *player, int c) {
    switch (c) {
        case 'k':
        case 'j':
        case 'h':
        case 'l':
        case 'y':
        case 'u':
        case 'b':
        case 'n': move_player(player, c); break;
        case ',': pickup_player(player); break;
        case '>': decend_player(player); break;
        default: load_msg_box("Unknown action %c. ", c); break;
    }

    /* has to be here because it redraws the fov */
    /* meaning it clears the old positions of monsters */
    reveal_partial_map(player->y, player->x, PLAYER_FOV);
}

void draw_player_stats(player_t player) {
    attrset(COLOR_PAIR(DEFAULT_COLOR_PAIR) | A_BOLD);
    move(MAP_HEIGHT, 0);

    char stat_line[81];
    snprintf(stat_line, 80, "$: %d, HP: %d, DMG: %d", player.money,
             player.health, player.damage);
    printw("%-80s", stat_line);
}
