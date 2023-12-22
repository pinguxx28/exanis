#ifndef ITEMS_H
#define ITEMS_H

#include <stdbool.h>

typedef struct {
    int x, y;
    char symbol;
    bool active;
} item_t;

item_t make_item(int x, int y, char symbol);
void append_item(item_t item);

void create_items(void);
void draw_items(int px, int py, float fov);

#endif /* ITEMS_H */
