#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#define MAP_HEIGHT 30
#define MAP_WIDTH 80
#define MAP_SIZE (MAP_HEIGHT * MAP_WIDTH)
#define NDRAWN (1 << 14)
#define N_SECTIONS 100
#define N_ROOMS 100

typedef struct {
    int x, y, w, h;
    bool active;
} section_t;

typedef struct {
    int x, y, w, h;
    bool active;
} room_t;

extern int section_ptr;
extern int room_ptr;
extern section_t sections[N_SECTIONS];
extern room_t rooms[N_ROOMS];
/* map index functions */
int get_mapch(int y, int x);
int get_seen_mapch(int y, int x);
void set_mapch(int y, int x, int ch);
void set_seen_mapch(int y, int x, int ch);

void print_map(void);
void reveal_partial_map(int y, int x, float fov);
void generate_map(void);

#endif /* MAP_H */
