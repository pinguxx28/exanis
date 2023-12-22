#include "../include/items.h"

#include "../include/colors.h"
#include "../include/debug.h"
#include "../include/helper.h"
#include "../include/map.h"
#include <ncurses.h>

#define N_ITEMS 1000
item_t items[N_ITEMS];

item_t make_item(int x, int y, char symbol) {
    return (item_t){
        .x = x,
        .y = y,
        .symbol = symbol,
        .active = true,
    };
}

void append_item(item_t item) {
    for (int i = 0; i < N_ITEMS; i++) {
        if (items[i].active) {
            continue;
        }

        items[i].x = item.x;
        items[i].y = item.y;
        items[i].symbol = item.symbol;
        items[i].active = true;
        return;
    }

    NC_ABORT("couldn't create item\n");
}

void create_items(void) {
    for (int i = 0; i < room_ptr; i++) {
        int x = random_i(rooms[i].x, rooms[i].x + rooms[i].w);
        int y = random_i(rooms[i].y, rooms[i].y + rooms[i].h);

        append_item(make_item(x, y, '$'));
    }
}

void draw_items(int px, int py, float fov) {
    for (int i = 0; i < N_ITEMS; i++) {
        if (!items[i].active) {
            continue;
        }

        switch (items[i].symbol) {
        case '$':
            attrset(COLOR_PAIR(MONEY_COLOR_PAIR));
            break;
        }

        attron(A_BOLD);

        if (distance(px, py, items[i].x, items[i].y) <= fov) {
            mvaddch(items[i].y, items[i].x, items[i].symbol);
        }
    }
}
