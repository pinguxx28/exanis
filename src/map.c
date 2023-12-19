#include "../include/map.h"

#include "../include/debug.h"
#include "../include/helper.h"
#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

int *map;
int *seen_map;

typedef struct {
    int x, y, w, h;
    bool active;
} room_t;

#define N_ROOMS 1000
room_t rooms[N_ROOMS];

int get_mapch(int y, int x) {
    if (y < 0 || y >= MAP_HEIGHT || x < 0 || x >= MAP_WIDTH) {
        return 0;
    }

    return map[y * MAP_WIDTH + x];
}
int get_seen_mapch(int y, int x) {
    /* count spaces outside the map as empty space */
    if (y < 0 || y >= MAP_HEIGHT || x < 0 || x >= MAP_WIDTH) {
        return 0;
    }

    return seen_map[y * MAP_WIDTH + x];
}
void set_mapch(int y, int x, int ch) { map[y * MAP_WIDTH + x] = ch; }
void set_seen_mapch(int y, int x, int ch) { seen_map[y * MAP_WIDTH + x] = ch; }

void print_map(void) {
    for (int i = 0; i < MAP_SIZE; i++) {
        int x = i % MAP_WIDTH;
        int y = i / MAP_WIDTH;

        mvaddch(y, x, get_mapch(y, x));

#if 0
        int tile = get_seen_mapch(y, x);

        /* only draw not drawn tiles */
        if (!(tile & NDRAWN)) {
            continue;
        }

        /* draw tile and unmark it not drawn */
        mvaddch(y, x, tile ^ NDRAWN);
        set_seen_mapch(y, x, tile ^ NDRAWN);
#endif
    }
}

void reveal_partial_map(int ypos, int xpos) {
    for (int i = 0; i < MAP_SIZE; i++) {
        int x = i % MAP_WIDTH;
        int y = i / MAP_WIDTH;

        float dist = sqrt(pow(ypos - y, 2) + pow(xpos - x, 2));

        if (dist < 2.5) {
            set_seen_mapch(y, x, get_mapch(y, x) | NDRAWN);
        }
    }
}

void init_maps(void) {
    map = calloc(MAP_SIZE, sizeof(int));
    seen_map = calloc(MAP_SIZE, sizeof(int));

    for (int i = 0; i < MAP_SIZE; i++) {
        map[i] = ' ';
    }

    for (int i = 0; i < N_ROOMS; i++) {
        rooms[i].active = false;
    }
}

room_t new_room(int x, int y, int w, int h) {
    return (room_t){
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .active = false,
    };
}

room_t create_room(int x, int y, int w, int h) {
    int i;
    for (i = 0; i < N_ROOMS; i++) {
        if (rooms[i].active) {
            continue;
        }

        rooms[i].x = x;
        rooms[i].y = y;
        rooms[i].w = w;
        rooms[i].h = h;
        rooms[i].active = true;
        return rooms[i];
    }

    fprintf(debug_file, "couldn't create room, this should never happen\n");
    fprintf(debug_file, "i=%d\n", i);
    return (room_t){0}; /* placeholder value because function must return */
}

void create_sections(room_t room) {
    room_t room1, room2;
    int split_horiz = rand() % 2;

    /* end if rooms is smaloop_lvl enough */
    if (room.w <= 15 || room.h <= 15) {
        create_room(room.x, room.y, room.w, room.h);
        return;
    }

    /* force split in the desired direction */
    float size_ratio = 2;
    if (room.w * size_ratio > room.h) {
        split_horiz = 0;
    }
    if (room.h * size_ratio > room.w) {
        split_horiz = 1;
    }

    float split_ratio = random_f(0.3, 0.7);

    if (split_horiz) {
        int split_point = room.h * split_ratio;
        room1 = new_room(room.x, room.y, room.w, split_point);
        room2 = new_room(room.x, room.y + split_point, room.w,
                         room.h - split_point);
    } else {
        int split_point = room.w * split_ratio;
        room1 = new_room(room.x, room.y, split_point, room.h);
        room2 = new_room(room.x + split_point, room.y, room.w - split_point,
                         room.h);
    }

    create_sections(room1);
    create_sections(room2);
}

void generate_map(void) {
    room_t map = {
        .x = 0,
        .y = 0,
        .w = MAP_WIDTH,
        .h = MAP_HEIGHT,
    };

    create_sections(map);
}
