#include "../include/monsters.h"

#include "../include/colors.h"
#include "../include/helper.h"
#include "../include/map.h"
#include <ncurses.h>
#include <stddef.h>

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
                       int speed) {
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
            append_monster(make_monster(y, x, 'x', 5, 1, 1));
        }
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
