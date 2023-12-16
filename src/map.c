#include "../include/map.h"

#include <assert.h>
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

int *map;
int *seen_map;

int get_mapch(int y, int x) { return map[y * MAP_WIDTH + x]; }
int get_seen_mapch(int y, int x) { return seen_map[y * MAP_WIDTH + x]; }
void set_mapch(int y, int x, int ch) { map[y * MAP_WIDTH + x] = ch; }
void set_seen_mapch(int y, int x, int ch) { seen_map[y * MAP_WIDTH + x] = ch; }

void load_map(const char *filepath) {
    map = calloc(MAP_HEIGHT * MAP_WIDTH, sizeof(int));
    seen_map = calloc(MAP_HEIGHT * MAP_WIDTH, sizeof(int));

    FILE *file = fopen(filepath, "r");
    assert(file != NULL);

    int c;
    int i = 0;
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

void reveal_partial_map(int ypos, int xpos) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            float dist = sqrt(pow(ypos - y, 2) + pow(xpos - x, 2));
            if (dist < 2.5) {
                set_seen_mapch(y, x, get_mapch(y, x) | NDRAWN);
            }
        }
    }
}
