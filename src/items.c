#include "../include/items.h"

#include "../include/colors.h"
#include "../include/debug.h"
#include "../include/helper.h"
#include "../include/map.h"
#include <ncurses.h>
#include <string.h>

#define MAX_ITEMS 100
item_t items[MAX_ITEMS];

item_t make_item(int y, int x, char symbol) {
    item_t item = {
        .y = y,
        .x = x,
        .symbol = symbol,
        .active = true,
    };

    item.name = malloc(24);

    switch (symbol) {
        case '$':
            item.amount = random_i(1, 20);
            item.damage = 0;
            strcpy(item.name, "money");
            break;
        case 't':
            item.amount = 1;
            item.damage = 4;
            strcpy(item.name, "sword");
            break;
        default:
            NC_ABORT("unrecognized symbol, in make_item, %c\n", symbol);
            break;
    }

    return item;
}

void append_item(item_t item) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items[i].active) continue;

        items[i] = item;
        return;
    }

    NC_ABORT("couldn't create item\n");
}

item_t *find_item(int y, int x) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].active) continue;

        if (y == items[i].y && x == items[i].x) {
            return &items[i];
        }
    }

    return NULL;
}

void remove_item(item_t *item) {
    item->active = false;
}

void create_items(void) {
    for (int i = 0; i < room_ptr; i++) {
        /* money */
        for (int j = 0; j < random_i(0, 4); j++) {
            int y, x;

            do {
                y = random_i(rooms[i].y, rooms[i].y + rooms[i].h);
                x = random_i(rooms[i].x, rooms[i].x + rooms[i].w);
            } while (find_item(y, x) != NULL);

            append_item(make_item(y, x, '$'));
        }
    }

    int y, x;
    int n = random_i(0, room_ptr);

    do {
        y = random_i(rooms[n].y, rooms[n].y + rooms[n].h);
        x = random_i(rooms[n].x, rooms[n].x + rooms[n].w);
    } while (find_item(y, x) != NULL);

    append_item(make_item(y, x, 't'));
}

void draw_items(int py, int px, float fov) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].active) continue;

        switch (items[i].symbol) {
            case '$': attrset(COLOR_PAIR(MONEY_COLOR_PAIR)); break;
            case 't': attrset(COLOR_PAIR(WEAPON_COLOR_PAIR)); break;
        }

        attron(A_BOLD);

        if (distance(py, px, items[i].y, items[i].x) <= fov) {
            mvaddch(items[i].y, items[i].x, items[i].symbol);
        }
    }
}
