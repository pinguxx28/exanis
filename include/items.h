#ifndef ITEMS_H
#define ITEMS_H

#include <stdbool.h>
#include "weapon.h"
#include "armor.h"

typedef enum {
    WEAPON,
	ARMOR,
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
    weapon_t weapon; 
	armor_t armor;

} item_t;

item_t make_item(int y, int x, char symbol);
void append_item(item_t item);
item_t *find_item(int y, int x);
void remove_item(item_t *item);

void create_items(void);
void draw_items(int py, int px, float fov);

#endif /* ITEMS_H */
