#include "../include/items.h"

#include "../include/colors.h"
#include "../include/debug.h"
#include "../include/helper.h"
#include "../include/map.h"
#include <ncurses.h>

#define N_ITEMS 1000
item_t items[N_ITEMS];

item_t make_item(int y, int x, int amount, char symbol) {
    return (item_t){
        .y = y,
        .x = x,
        .amount = amount,
        .symbol = symbol,
        .active = true,
    };
}

void append_item(item_t item) {
    for (int i = 0; i < N_ITEMS; i++) {
        if (items[i].active) continue;

        items[i] = item;
        return;
    }

    NC_ABORT("couldn't create item\n");
}

item_t *find_item(int y, int x) {
    for (int i = 0; i < N_ITEMS; i++) {
        if (!items[i].active) continue;

        if (y == items[i].y && x == items[i].x) {
            return &items[i];
        }
    }

    NC_ABORT("couldn't find item\n");
}

void remove_item(item_t *item) {
    item->active = false;
}

void create_items(void) {
    for (int i = 0; i < room_ptr; i++) {
        /* OBS: they can spawn on eachother */
        for (int j = 0; j < random_i(0, 4); j++) {
            int y = random_i(rooms[i].y, rooms[i].y + rooms[i].h);
            int x = random_i(rooms[i].x, rooms[i].x + rooms[i].w);
            append_item(make_item(y, x, random_i(1, 20), '$'));
        }
    }
}

void draw_items(int py, int px, float fov) {
    for (int i = 0; i < N_ITEMS; i++) {
        if (!items[i].active) continue;

        switch (items[i].symbol) {
            case '$': attrset(COLOR_PAIR(MONEY_COLOR_PAIR)); break;
        }

        attron(A_BOLD);

        if (distance(py, px, items[i].y, items[i].x) <= fov) {
            mvaddch(items[i].y, items[i].x, items[i].symbol);
        }
    }
}
