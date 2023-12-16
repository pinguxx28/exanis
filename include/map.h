#ifndef MAP_H
#define MAP_H

#include "defs.h"
#define MAP_HEIGHT 10
#define MAP_WIDTH 40
#define NDRAWN (1 << 14)

u16 get_mapch(i16 y, i16 x);
u16 get_seen_mapch(i16 y, i16 x);
void set_mapch(i16 y, i16 x, u16 ch);
void set_seen_mapch(i16 y, i16 x, u16 ch);

void load_map(const char *filepath);
void print_map(void);
void reveal_partial_map(i16 y, i16 x);

#endif /* MAP_H */
