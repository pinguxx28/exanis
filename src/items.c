#include "items.h"

#include <ncurses.h>
#include <string.h>
#include "colors.h"
#include "debug.h"
#include "helper.h"
#include "map.h"

#define MAX_ITEMS 100
item_t items[MAX_ITEMS];

item_t make_item(int y, int x, char symbol) {
    item_t item = {
        .y = y,
        .x = x,
        .symbol = symbol,
        .active = true,
    };

    switch (symbol) {
        case '$':
            item.type = MONEY;
            item.amount = random_i(1, 20);

			item.name = malloc(strlen("money") + 1);
            strcpy(item.name, "money");
            break;
        case 't':
            item.type = WEAPON;
            item.amount = 1;
            item.weapon = make_weapon(SWORD);

			item.name = malloc(strlen(item.weapon.name) + 1);
            strcpy(item.name, item.weapon.name);
            break;
        default:
            NC_ABORT("unrecognized symbol, in make_item, %c\n", symbol);
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
        if (y == items[i].y && x == items[i].x) return &items[i];
    }

    return NULL;
}

void remove_item(item_t *item) {
    item->active = false;
}

void create_items(void) {
	/* TODO: not the most elegant code but really can't be bothered rn tbh */
    for (int i = 0; i < num_rooms; i++) {
        /* money */
        for (int j = 0; j < random_i(0, 4); j++) {
            int y, x;

            do {
                y = random_i(rooms[i].y, rooms[i].y + rooms[i].h);
                x = random_i(rooms[i].x, rooms[i].x + rooms[i].w);
            } while (find_item(y, x) != NULL);

            append_item(make_item(y, x, '$'));
        }

        /* weapon */
        /* 20% chance to spawn a weapon in a room */
        if (random_i(0, 5) != 0) continue;

        int y, x;

        do {
            y = random_i(rooms[i].y, rooms[i].y + rooms[i].h);
            x = random_i(rooms[i].x, rooms[i].x + rooms[i].w);
        } while (find_item(y, x) != NULL);

        append_item(make_item(y, x, 't'));
    }
}

void draw_items(int py, int px, float fov) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (!items[i].active) continue;

        attrset(A_BOLD);

        switch (items[i].symbol) {
            case '$': attron(COLOR_PAIR( MONEY_COLOR_PAIR)); break;
            case 't': attron(COLOR_PAIR(WEAPON_COLOR_PAIR)); break;
        }


        if (distance(py, px, items[i].y, items[i].x) <= fov) {
            mvaddch(items[i].y, items[i].x, items[i].symbol);
        }
    }
}
