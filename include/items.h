#ifndef ITEMS_H
#define ITEMS_H

#include <stdbool.h>
#include "../include/weapon.h"

typedef enum {
    WEAPON,
    MONEY,
} item_type;

typedef struct {
    int x, y;
    char symbol;
    char *name;
    bool active;

    /* type specific values */
    item_type type;
    int amount;
    weapon_t weapon; /* only set when type == WEAPON */
} item_t;

item_t make_item(int y, int x, char symbol);
void append_item(item_t item);
item_t *find_item(int y, int x);
void remove_item(item_t *item);

void create_items(void);
void draw_items(int py, int px, float fov);

#endif /* ITEMS_H */
