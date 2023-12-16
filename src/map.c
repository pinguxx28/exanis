#include "../include/map.h"

#include <assert.h>
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

u16 *map;
u16 *seen_map;

u16 get_mapch(i16 y, i16 x) { return map[y * MAP_WIDTH + x]; }
u16 get_seen_mapch(i16 y, i16 x) { return seen_map[y * MAP_WIDTH + x]; }
void set_mapch(i16 y, i16 x, u16 ch) { map[y * MAP_WIDTH + x] = ch; }
void set_seen_mapch(i16 y, i16 x, u16 ch) { seen_map[y * MAP_WIDTH + x] = ch; }

void load_map(const char *filepath) {
    map = calloc(MAP_HEIGHT * MAP_WIDTH, sizeof(u16));
    seen_map = calloc(MAP_HEIGHT * MAP_WIDTH, sizeof(u16));

    FILE *file = fopen(filepath, "r");
    assert(file != NULL);

    i8 c;
    u32 i = 0;
    while ((c = getc(file)) != EOF) {
        if (c == '\n')
            continue;

        map[i] = c;
        seen_map[i] = ' ';
        i++;
    }
}

void print_map(void) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            int ch = get_seen_mapch(y, x);
            if (ch & NDRAWN) {
                mvaddch(y, x, ch ^ NDRAWN);
                set_seen_mapch(y, x, ch ^ NDRAWN);
            }
        }
    }
}

void reveal_partial_map(i16 ypos, i16 xpos) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            float dist = sqrt(pow(ypos - y, 2) + pow(xpos - x, 2));
            if (dist < 2.5) {
                set_seen_mapch(y, x, get_mapch(y, x) | NDRAWN);
            }
        }
    }
}
