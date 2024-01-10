#include "monsters.h"

#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "map.h"
#include "debug.h"
#include "items.h"
#include "colors.h"
#include "helper.h"
#include "msg_box.h"

monster_t monsters[MAX_MONSTERS];

monster_t *find_monster(int y, int x) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) continue;
        if (y == monsters[i].y && x == monsters[i].x) return &monsters[i];
    }

    return NULL;
}

monster_t make_monster(int y, int x, char symbol) {
    monster_t monster = {
        .y = y,
        .x = x,
        .symbol = symbol
    };

    switch (symbol) {
        case 'x':
            monster.name = malloc(strlen("Bug") + 1);
            strcpy(monster.name, "Bug");

            monster.health = 2;
            monster.damage = 1;
            monster.speed = 2.0 / 3.0;
            break;
        case 'o':
            monster.name = malloc(strlen("Orc") + 1);
            strcpy(monster.name, "Orc");

            monster.health = 6;
            monster.damage = 3;
            monster.speed = 1.0 / 2.0;
            break;
        default:
			NC_ABORT("unrecognized monster symbol, %c", symbol);
    }

    return monster;
}

void append_monster(monster_t monster) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (monsters[i].active) continue;

        monsters[i] = monster;
        monsters[i].active = true;
        break;
    }
}

void create_monsters(void) {
	for (int i = 0; i < MAX_MONSTERS; i++) {
		monsters[i].active = false;
	}

    for (int i = 0; i < num_rooms; i++) {
		int y, x;

        int amount = random_i(1, 1 + rooms[i].h * rooms[i].w / 50);
        for (int j = 0; j < amount; j++) {
            y = random_i(rooms[i].y, rooms[i].y + rooms[i].h);
            x = random_i(rooms[i].x, rooms[i].x + rooms[i].w);
            append_monster(make_monster(y, x, 'x'));
        }

		bool create_orc = random_i(0, 4) == 0;
        if (create_orc) {
            y = random_i(rooms[i].y, rooms[i].y + rooms[i].h);
            x = random_i(rooms[i].x, rooms[i].x + rooms[i].w);
            append_monster(make_monster(y, x, 'o'));
        }
    }
}

static void handle_monster_movement(monster_t *monster, int py, int px) {
    int newy = monster->y;
    int newx = monster->x;

    /* monsters walk randomly if they can't see the player */
    /* otherwise the monsters walk towards the player */
    if (distance(py, px, monster->y, monster->x) >= MONSTERS_FOV) {
        if (rand() % 2) {
            newx += rand() % 2 ? 1 : -1;
        } else {
            newy += rand() % 2 ? 1 : -1;
        }
    } else {
        /* walk in the furthest away direcetion */
        /* if both directions are the same, walk randomly */
        /* but always move towards the player */
        bool move_horiz = abs(newx - px) > abs(newy - py);
        if (abs(newx - px) == abs(newy - py)) {
            move_horiz = rand() % 2;
        }

        if (move_horiz) {
            if (newx < px) newx++;
            else if (newx > px) newx--;
        } else {
            if (newy < py) newy++;
            else if (newy > py) newy--;
        }
    }

	bool valid_tile =
		get_mapch(newy, newx) != '-' &&
		get_mapch(newy, newx) != '|' &&
        get_mapch(newy, newx) != '#';
	bool should_move =
		rand() % 101 <= monsters->speed * 100 &&
        distance(py, px, newy, newx) >= 1;

    if (valid_tile && should_move) {
        monster->y = newy;
        monster->x = newx;
    }
}

static void handle_monster_punch(monster_t *monster, int *health) {
    int old_health = *health;
    *health -= monster->damage;
    *health = max(*health, 0);

    load_msg_box("%s hit you HP: %d->%d! ",
		monster->name, old_health, *health);
}

void update_monsters(int py, int px, int *health) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) continue;

        if (monsters[i].health <= 0) {
            monsters[i].active = false;
            load_msg_box("%s died. ", monsters[i].name);
            continue;
        }

        handle_monster_movement(&monsters[i], py, px);

		if (distance(py, px, monsters[i].y, monsters[i].x) == 1) {
			handle_monster_punch(&monsters[i], health);
		}
    }
}

void draw_monsters(int py, int px, float fov) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) continue;
        if (distance(py, px, monsters[i].y, monsters[i].x) >= fov) continue;

        switch (monsters[i].symbol) {
            case 'x': attrset(COLOR_PAIR(MONSTERS_COLOR_PAIR) | A_BOLD); break;
            case 'o': attrset(COLOR_PAIR(ORC_COLOR_PAIR)      | A_BOLD); break;
        }

        mvaddch(monsters[i].y, monsters[i].x, monsters[i].symbol);
    }
}
