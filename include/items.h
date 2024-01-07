#ifndef ITEMS_H
#define ITEMS_H

#include <stdbool.h>
#include "weapon.h"

typedef enum {
    WEAPON,
    MONEY,
} item_type;

typedef struct {
    int x, y;
    int amount;
    char *name;
    char symbol;
    bool active;

    /* type specific values */
    item_type type;
	/* only set when type is WEAPON */
	/* otherwise it is undefined */
    weapon_t weapon; 
} item_t;

item_t make_item(int y, int x, char symbol);
void append_item(item_t item);
item_t *find_item(int y, int x);
void remove_item(item_t *item);

void create_items(void);
void draw_items(int py, int px, float fov);

#endif /* ITEMS_H */
