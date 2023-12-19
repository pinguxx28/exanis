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
    int ll; /* loop level */
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
        rooms[i].ll = 0;
    }
}

room_t new_room(int x, int y, int w, int h, int ll) {
    return (room_t){
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .active = false, /* doesn't really matter */
        .ll = ll + 1,
    };
}

room_t create_room(int x, int y, int w, int h) {
    static int c = 'a';
    c++;

    for (int i = y; i < y + h; i++) {
        for (int j = x; j < x + w; j++) {
            set_mapch(i, j, c);
        }
    }

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
    return rooms[0]; /* placeholder value */
}

void bsp(room_t room) {
    room_t room1, room2;
    int split_horiz = rand() % 2;

    /* end if rooms is small enough */
    // if (room.w * room.h <= (rand() % 100) + 100) {
    // if (room.ll == 4) {
    if (room.w <= 15 || room.h <= 15) {
        create_room(room.x, room.y, room.w, room.h);
        return;
    }

    /* force split in the desired direction */
    float ratio = 2;
    if (room.w * ratio > room.h) {
        split_horiz = 0;
    }
    if (room.h * ratio > room.w) {
        split_horiz = 1;
    }

    // float offset = (rand() % 6) / 10.0 + 0.25;
    float offset = random_f(0.3, 0.7);
    // fprintf(debug_file, "%.2f\n", offset);

    if (split_horiz) {
        int split_point = room.h * offset;
        room1 = new_room(room.x, room.y, room.w, split_point, room.ll);
        room2 = new_room(room.x, room.y + split_point, room.w,
                         room.h - split_point, room.ll);
    } else {
        int split_point = room.w * offset;
        room1 = new_room(room.x, room.y, split_point, room.h, room.ll);
        room2 = new_room(room.x + split_point, room.y, room.w - split_point,
                         room.h, room.ll);
    }

    bsp(room1);
    bsp(room2);
}

void generate_map(void) {
    room_t map = {
        .x = 0,
        .y = 0,
        .w = MAP_WIDTH,
        .h = MAP_HEIGHT,
        .ll = 0,
    };

    bsp(map);
}
