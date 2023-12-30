#include "../include/monsters.h"

#include "../include/colors.h"
#include "../include/debug.h"
#include "../include/helper.h"
#include "../include/items.h"
#include "../include/map.h"
#include "../include/msg_box.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

monster_t monsters[MAX_MONSTERS];

monster_t *find_monster(int y, int x) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) continue;

        if (y == monsters[i].y && x == monsters[i].x) {
            return &monsters[i];
        }
    }

    return NULL;
}

monster_t make_monster(int y, int x, char symbol) {
    monster_t monster = {
        .y = y,
        .x = x,
        .symbol = symbol,
        .active = true, /* isn't really required */
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
        default: NC_ABORT("unrecognized monster symbol, %c", symbol); break;
    }

    return monster;
}

void append_monster(monster_t monster) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (monsters[i].active) continue;

        monsters[i] = monster;

        /* just make sure that we set it to active */
        /* doesn't harm anyone */
        monsters[i].active = true;
        break;
    }
}

void init_monsters(void) {
    for (int i = 0; i < room_ptr; i++) {
        int amount = random_i(1, 1 + rooms[i].h * rooms[i].w / 50);

        for (int j = 0; j < amount; j++) {
            int y = random_i(rooms[i].y, rooms[i].y + rooms[i].h);
            int x = random_i(rooms[i].x, rooms[i].x + rooms[i].w);
            append_monster(make_monster(y, x, 'x'));
        }

        if (random_i(0, 4) == 0) {
            int y = random_i(rooms[i].y, rooms[i].y + rooms[i].h);
            int x = random_i(rooms[i].x, rooms[i].x + rooms[i].w);
            append_monster(make_monster(y, x, 'o'));
        }
    }
}

static void move_monster(monster_t *monster, int py, int px) {
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

    if (get_mapch(newy, newx) != '-' && get_mapch(newy, newx) != '|' && get_mapch(newy, newx) != '#' &&
			rand() % 101 <= monsters->speed * 100 &&
        distance(py, px, newy, newx) >= 1) {
        monster->y = newy;
        monster->x = newx;
    }
}

static void punch_monster(monster_t *monster, int py, int px, int *health) {
    if (distance(py, px, monster->y, monster->x) > 1) return;

    int old_health = *health;
    *health -= monster->damage;
    *health = max(*health, 0);

    load_msg_box("%s hit you HP: %d->%d! ", monster->name, old_health, *health);
}

void update_monsters(int py, int px, int *health) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) continue;

        if (monsters[i].health <= 0) {
            monsters[i].active = false;
            load_msg_box("%s died. ", monsters[i].name);
            continue;
        }

        move_monster(&monsters[i], py, px);
        punch_monster(&monsters[i], py, px, health);
    }
}

void draw_monsters(int py, int px, float fov) {
    for (int i = 0; i < MAX_MONSTERS; i++) {
        if (!monsters[i].active) continue;
        if (distance(py, px, monsters[i].y, monsters[i].x) >= fov) continue;

        switch (monsters[i].symbol) {
            case 'x': attrset(COLOR_PAIR(MONSTERS_COLOR_PAIR) | A_BOLD); break;
            case 'o': attrset(COLOR_PAIR(ORC_COLOR_PAIR) | A_BOLD); break;
        }

        mvaddch(monsters[i].y, monsters[i].x, monsters[i].symbol);
    }
}
