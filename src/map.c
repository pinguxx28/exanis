#include "../include/map.h"

#include <assert.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

u16 *map;

u16 map_index(i16 y, i16 x) {
    u16 i = map[y * MAP_WIDTH + x];
    return i;
}

void load_map(const char *filepath) {
    map = calloc(MAP_HEIGHT * MAP_WIDTH, sizeof(u16));

    FILE *file = fopen(filepath, "r");
    assert(file != NULL);

    i8 c;
    u32 i = 0;
    while ((c = getc(file)) != EOF) {
        if (c == '\n')
            continue;

        map[i] = c;
        i++;
    }
}

void print_map(void) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            mvaddch(y, x, map_index(y, x));
        }
    }
}
