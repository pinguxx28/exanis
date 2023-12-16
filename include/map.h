#ifndef MAP_H
#define MAP_H

#include "defs.h"
#define MAP_HEIGHT 10
#define MAP_WIDTH 40

u16 map_index(i16 y, i16 x);
void load_map(const char *filepath);
void print_map(void);

#endif /* MAP_H */
