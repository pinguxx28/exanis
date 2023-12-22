#ifndef ITEMS_H
#define ITEMS_H

#include <stdbool.h>

typedef struct {
    int x, y;
    int amount;
    char symbol;
    bool active;
} item_t;

item_t make_item(int x, int y, int amount, char symbol);
void append_item(item_t item);
item_t *find_item(int y, int x);
void remove_item(item_t *item);

void create_items(void);
void draw_items(int px, int py, float fov);

#endif /* ITEMS_H */
