#include "player.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>

#include "map.h"
#include "debug.h"
#include "items.h"
#include "colors.h"
#include "helper.h"
#include "msg_box.h"
#include "monsters.h"

static void set_new_player_pos(player_t *player) {
	room_t *room = &rooms[random_i(0, num_rooms)];
	bool on_item, on_monster;

    do {
        player->y = random_i(room->y, room->y + room->h);
        player->x = random_i(room->x, room->x + room->w);
		on_item = find_item(player->y, player->x) != NULL;
		on_monster = find_monster(player->y, player->x) != NULL;
    } while (on_item || on_monster);
}

player_t *init_player(void) {
    player_t *player = malloc(sizeof(player_t));

	player->money = 0;

    player->health = 15;
    player->damage = 2;
    player->speed = 1;

    player->weapon = make_weapon(FIST);

	set_new_player_pos(player);

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
	bool valid_tile = ch != '-' && ch != '|' && ch != ' ';

    monster_t *monster = find_monster(newy, newx);


	/* if we try move onto a tile containing a monster */
	/* we hit the monster instead of moving */
    if (monster != NULL) {
        int old_health = monster->health;
        monster->health -= player->damage;
        monster->health = max(monster->health, 0);

		load_msg_box("You hit %s with a %s, its HP: %d->%d! ",
			monster->name, player->weapon.name, old_health, monster->health);
		
		if (monster->health > 0) return;

		load_msg_box("%s died. ", monster->name);
		player->exp += monster->exp_gain;
		monster->active = false;
    } else if (valid_tile) {
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

    remove_item(item);

    switch (item->type) {
        case MONEY:
            player->money += item->amount;
            load_msg_box("Cha-ching +$%d. ", item->amount);
            break;
        case WEAPON:
			player->weapon = item->weapon;
			player->damage = item->weapon.damage;
            load_msg_box("Picked up and eqiupped a %s. ", item->name);
            break;
		case ARMOR:
			player->armor = item->armor;
			player->protection = item->armor.protection;
            load_msg_box("Picked up and eqiupped a %s. ", item->name);
			break;
        default:
			NC_ABORT("Unknown item: %s (type: %d)\n", item->name, item->type);
    }
}

static void decend_player(player_t *player) {
    if (get_mapch(player->y, player->x) != '>') {
        load_msg_box("Can't decend here. ");
    }

	clear();
	int rows, cols;
	getmaxyx(stdscr, rows, cols);
	int y = rows / 2;
	int x;
	

	const char str[] = "F L O O R   C O M P L E T E D";
	x = (cols - strlen(str)) / 2;
	mvprintw(y++, x, str);


	const int maxlen = 80;
	char str2[maxlen];
	snprintf(str2, maxlen, "money: $%d, exp: %d", player->money, player->exp);

	x = (cols - strlen(str2)) / 2;
	mvprintw(y++, x, str2);


	getch();

	clear();
    create_map();
    create_items();
    create_monsters();
	set_new_player_pos(player);
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
		case ' ': break; /* do nothing */
        default: load_msg_box("Unknown action %c. ", c); break;
    }

    /* clears the old positions of monsters */
    reveal_partial_map(player->y, player->x, PLAYER_FOV);
}

void draw_player_stats(player_t player) {
    attrset(COLOR_PAIR(DEFAULT_COLOR_PAIR) | A_BOLD);

    char stat_line[81];
    snprintf(stat_line, 80, "$: %d, HP: %d, DMG: %d, PROT: %d, EXP: %d",
			player.money, player.health, player.damage, player.protection, player.exp);
    mvprintw(MAP_HEIGHT, 0, "%-80s", stat_line);
}
