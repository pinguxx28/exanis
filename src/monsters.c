#include "../include/monsters.h"

#include "../include/colors.h"
#include "../include/helper.h"
#include "../include/map.h"
#include "../include/msg_box.h"
#include <ncurses.h>
#include <stdlib.h>

monster_t monsters[N_MONSTERS];

monster_t *find_monster(int y, int x) {
    for (int i = 0; i < N_MONSTERS; i++) {
        if (!monsters[i].active) continue;

        if (y == monsters[i].y && x == monsters[i].x) {
            return &monsters[i];
        }
    }

    return NULL;
}

monster_t make_monster(int y, int x, char symbol, int health, int damage,
                       float speed) {
    return (monster_t){
        .y = y,
        .x = x,
        .symbol = symbol,
        .health = health,
        .damage = damage,
        .speed = speed,
        .active = true /* isin't required */
    };
}

void append_monster(monster_t monster) {
    for (int i = 0; i < N_MONSTERS; i++) {
        if (monsters[i].active) continue;

        monsters[i] = monster;

        /* just make sure that we set it to active, doesn't harm anyone */
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
            append_monster(make_monster(y, x, 'x', 5, 1, 2.0 / 3.0));
        }
    }
}

static void move_monster(monster_t *monster, int py, int px, int *health) {
    int newy = monster->y;
    int newx = monster->x;

    if (distance(py, px, monster->y, monster->x) >= MONSTERS_FOV) {
        bool move_horiz = rand() % 2;
        if (move_horiz) newx += rand() % 2 ? 1 : -1;
        else newy += rand() % 2 ? 1 : -1;
    } else {
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

    int ch = get_mapch(newy, newx);
    if (newy == py && newx == px) {
        int old_health = *health;
        *health -= monster->damage;
        if (*health <= 0) *health = 0;
        load_msg_box("Monster hit you HP: %d->%d! ", old_health, *health);
    } else if (ch == '.' && rand() % 101 <= monsters->speed * 100) {
        monster->y = newy;
        monster->x = newx;
    }
}

void update_monsters(int py, int px, int *health) {
    for (int i = 0; i < N_MONSTERS; i++) {
        if (!monsters[i].active) continue;

        if (monsters[i].health <= 0) {
            monsters[i].active = false;
            load_msg_box("Monster died. ");
            continue;
        }

        move_monster(&monsters[i], py, px, health);
    }
}

void draw_monsters(int py, int px, float fov) {
    attrset(COLOR_PAIR(MONSTERS_COLOR_PAIR) | A_BOLD);
    for (int i = 0; i < N_MONSTERS; i++) {
        if (!monsters[i].active) continue;

        if (distance(py, px, monsters[i].y, monsters[i].x) < fov) {
            mvaddch(monsters[i].y, monsters[i].x, monsters[i].symbol);
        }
    }
}
