#ifndef MAP_H
#define MAP_H

#define MAP_HEIGHT 10
#define MAP_WIDTH 40
#define MAP_SIZE (MAP_HEIGHT * MAP_WIDTH)
#define NDRAWN (1 << 14)

int get_mapch(int y, int x);
int get_seen_mapch(int y, int x);
void set_mapch(int y, int x, int ch);
void set_seen_mapch(int y, int x, int ch);

void load_map(const char *filepath);
void print_map(void);
void reveal_partial_map(int y, int x);

#endif /* MAP_H */
