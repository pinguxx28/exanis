#include "player.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "colors.h"
#include "debug.h"
#include "helper.h"
#include "items.h"
#include "map.h"
#include "monsters.h"
#include "msg_box.h"

player_t *init_player(void) {
    player_t *player = malloc(sizeof(player_t));

	player->money = 0;

    player->health = 15;
    player->damage = 2;
    player->speed = 1;

    player->weapon = make_weapon(NONE);

	/* find a random empty square to stand on */
	/* WARNING: could take time */
    do {
        player->y = random_i(0, MAP_HEIGHT);
        player->x = random_i(0, MAP_WIDTH);
    } while (
		get_mapch(player->y, player->x) != '.' &&
		find_monster(player->y, player->x) == NULL &&
		find_item(player->y, player->x));

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

static void move_player(player_t *player, int c) {
    int newy = player->y;
    int newx = player->x;

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

    /* only handle movement if we moved */
    if (newx == player->x && newy == player->y) return;

    int ch = get_mapch(newy, newx);
    monster_t *monster = find_monster(newy, newx);

	/* if we try move onto a tile containing a monster */
	/* we hit the monster instead of moving */
    if (monster != NULL) {
        int old_health = monster->health;
        monster->health -= player->damage;
        monster->health = max(monster->health, 0);

		load_msg_box("You hit %s with a %s, its HP: %d->%d! ",
			monster->name, player->weapon.name, old_health, monster->health);
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
			player->weapon = item->weapon;
			player->damage = item->weapon.damage;
            load_msg_box("Picked up and eqiupped a %s. ", item->weapon.name);
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

    char stat_line[81];
    snprintf(stat_line, 80, "$: %d, HP: %d, DMG: %d",
			player.money, player.health, player.damage);
    mvprintw(MAP_HEIGHT, 0, "%-80s", stat_line);
}
