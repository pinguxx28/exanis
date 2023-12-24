#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#define MAP_HEIGHT 30
#define MAP_WIDTH 80
#define MAP_SIZE (MAP_HEIGHT * MAP_WIDTH)
#define NDRAWN (1 << 14)
#define SEEN (1 << 13)
#define UNSEEN (1 << 12)
#define MAX_SECTIONS 100
#define MAX_ROOMS 100

typedef struct {
    int y, x;
    int h, w;
    bool active;
} section_t;

typedef struct {
    int y, x;
    int h, w;
    bool active;
} room_t;

extern int section_ptr;
extern int room_ptr;
extern section_t sections[MAX_SECTIONS];
extern room_t rooms[MAX_ROOMS];
/* map index functions */
int get_mapch(int y, int x);
int get_seen_mapch(int y, int x);
void set_mapch(int y, int x, int ch);
void set_seen_mapch(int y, int x, int ch);

void print_map(void);
void reveal_partial_map(int y, int x, float fov);
void generate_map(void);

#endif /* MAP_H */
