#include "../include/map.h"

#include "../include/debug.h"
#include "../include/helper.h"
#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>

#define SECTION_MIN_W 15
#define SECTION_MIN_H 15

int *map;
int *seen_map;

typedef struct {
    int x, y, w, h;
    bool active;
} section_t;

#define N_SECTIONS 1000
section_t sections[N_SECTIONS];

typedef struct {
    int x, y, w, h;
    bool active;
} room_t;

#define N_ROOMS 500
room_t rooms[N_ROOMS];

int get_mapch(int y, int x) {
    if (y < 0 || y >= MAP_HEIGHT || x < 0 || x >= MAP_WIDTH) {
        return 0;
    }

    return map[y * MAP_WIDTH + x];
}
int get_seen_mapch(int y, int x) {
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

        int tile = get_seen_mapch(y, x);

        /* only draw not drawn tiles */
        if (!(tile & NDRAWN)) {
            continue;
        }

        /* draw tile and unmark it not drawn */
        mvaddch(y, x, tile ^ NDRAWN);
        set_seen_mapch(y, x, tile ^ NDRAWN);
    }
}

void reveal_partial_map(int ypos, int xpos) {
    for (int i = 0; i < MAP_SIZE; i++) {
        int x = i % MAP_WIDTH;
        int y = i / MAP_WIDTH;

        float dist = distance(x, y, xpos, ypos);

        if (dist < 3.5) {
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

    for (int i = 0; i < N_SECTIONS; i++) {
        sections[i].active = false;
    }
}

section_t new_section(int x, int y, int w, int h) {
    return (section_t){
        .x = x,
        .y = y,
        .w = w,
        .h = h,
        .active = false,
    };
}

section_t create_section(int x, int y, int w, int h) {
    int i; /* debugging purposes */

    for (i = 0; i < N_SECTIONS; i++) {
        if (sections[i].active) {
            continue;
        }

        sections[i].x = x;
        sections[i].y = y;
        sections[i].w = w;
        sections[i].h = h;
        sections[i].active = true;
        return sections[i];
    }

    fprintf(debug_file, "couldn't create section, this should never happen\n");
    fprintf(debug_file, "i=%d\n", i);
    return (section_t){0}; /* placeholder value because function must return */
}

void create_sections(section_t section) {
    section_t section1, section2;
    int split_horiz = rand() % 2;

    /* end if sections is small enough */
    if (section.w <= SECTION_MIN_W || section.h <= SECTION_MIN_H) {
        create_section(section.x, section.y, section.w, section.h);
        return;
    }

    /* force split in the desired direction */
    float size_ratio = 2;
    if (section.w * size_ratio > section.h) {
        split_horiz = 0;
    }
    if (section.h * size_ratio > section.w) {
        split_horiz = 1;
    }

    float split_ratio = random_f(0.3, 0.7);

    if (split_horiz) {
        int split_point = random_i(7, section.h - 7);
        section1 = new_section(section.x, section.y, section.w, split_point);
        section2 = new_section(section.x, section.y + split_point, section.w,
                               section.h - split_point);
    } else {
        int split_point = random_i(7, section.w - 7);
        section1 = new_section(section.x, section.y, split_point, section.h);
        section2 = new_section(section.x + split_point, section.y,
                               section.w - split_point, section.h);
    }

    create_sections(section1);
    create_sections(section2);
}

room_t new_room(int x, int y, int w, int h) {
    for (int i = 0; i < N_ROOMS; i++) {
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

    fprintf(debug_file, "no space for more rooms, this should never happen\n");
    exit(1);
}

void create_rooms(void) {
    for (int i = 0; i < N_SECTIONS; i++) {
        if (!sections[i].active) {
            break;
        }

        int x1, y1, x2, y2;

        /* clang-format off */
        do {
            x1 = random_i(sections[i].x + 1, sections[i].x + sections[i].w / 2 - 2);
            y1 = random_i(sections[i].y + 1, sections[i].y + sections[i].h / 2 - 2);
            x2 = random_i(sections[i].x + sections[i].w / 2 + 2, sections[i].x + sections[i].w - 1);
            y2 = random_i(sections[i].y + sections[i].h / 2 + 2, sections[i].y + sections[i].h - 1);
        } while (x2 - x1 < 4 || y2 - y1 < 4);
        /* clang-format on */

        new_room(x1, y1, x2 - x1, y2 - y1);
    }
}

void fill_rooms(void) {
    for (int i = 0; i < N_ROOMS; i++) {
        if (!rooms[i].active) {
            break;
        }

        /* sides of room */
        for (int y = rooms[i].y; y < rooms[i].y + rooms[i].h; y++) {
            set_mapch(y, rooms[i].x - 1, '|');
            set_mapch(y, rooms[i].x + rooms[i].w, '|');
        }
        /* top and bottom */
        for (int x = rooms[i].x - 1; x < rooms[i].x + rooms[i].w + 1; x++) {
            set_mapch(rooms[i].y - 1, x, '-');
            set_mapch(rooms[i].y + rooms[i].h, x, '-');
        }

        for (int x = rooms[i].x; x < rooms[i].x + rooms[i].w; x++) {

            for (int y = rooms[i].y; y < rooms[i].y + rooms[i].h; y++) {
                set_mapch(y, x, '.');
            }
        }
    }
}

void make_corridors(void) {
    for (int i = 0; i < N_ROOMS; i++) {
        if (!rooms[i].active) {
            break;
        }

        int cx1 = rooms[i].x + rooms[i].w / 2;
        int cy1 = rooms[i].y + rooms[i].h / 2;

        int j = i + 1;
        if (!rooms[j].active) {
            j = i;
        }

        int cx2 = rooms[j].x + rooms[j].w / 2;
        int cy2 = rooms[j].y + rooms[j].h / 2;

        while (cx1 != cx2) {
            int c = '.';
            int ch = get_mapch(cy1, cx1);

            if (ch != '.') {
                c = '#';
            }

            set_mapch(cy1, cx1, c);
            cx1 += (cx1 < cx2) ? 1 : -1;
        }

        while (cy1 != cy2) {
            int c = '.';
            int ch = get_mapch(cy1, cx1);

            if (ch != '.') {
                c = '#';
            }

            set_mapch(cy1, cx1, c);
            cy1 += (cy1 < cy2) ? 1 : -1;
        }
    }
}

void generate_map(void) {
    section_t map = {
        .x = 0,
        .y = 0,
        .w = MAP_WIDTH,
        .h = MAP_HEIGHT,
    };

    create_sections(map);
    create_rooms();
    fill_rooms();
    make_corridors();
    fprintf(debug_file, "%d %d %d %d\n", rooms[0].x, rooms[0].y, rooms[0].w,
            rooms[0].h);
}
