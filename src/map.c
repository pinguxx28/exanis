#include "../include/map.h"

#include "../include/colors.h"
#include "../include/debug.h"
#include "../include/helper.h"
#include <ncurses.h>

#define SECTION_MIN_W 15
#define SECTION_MIN_H 15

#define CHECK_BOUNDS(y, x, fname)                                              \
    do {                                                                       \
        if ((y) < 0 || (y) >= MAP_HEIGHT || (x) < 0 || (x) >= MAP_WIDTH) {     \
            NC_ABORT("out of bounds\ny: %d, x: %d, inside of %s\n", (y), (x),  \
                     (fname));                                                 \
        }                                                                      \
    } while (0)

int *map;
int *seen_map;

int section_ptr = 0;
int room_ptr = 0;
section_t sections[MAX_SECTIONS];
room_t rooms[MAX_ROOMS];

int get_mapch(int y, int x) {
    CHECK_BOUNDS(y, x, "get_mapch");
    return map[y * MAP_WIDTH + x];
}

int get_seen_mapch(int y, int x) {
    CHECK_BOUNDS(y, x, "get_seen_mapch");
    return seen_map[y * MAP_WIDTH + x];
}

void set_mapch(int y, int x, int ch) {
    CHECK_BOUNDS(y, x, "set_mapch");
    map[y * MAP_WIDTH + x] = ch;
}

void set_seen_mapch(int y, int x, int ch) {
    CHECK_BOUNDS(y, x, "set_seen_mapch");
    seen_map[y * MAP_WIDTH + x] = ch;
}

void print_map(void) {
    attrset(COLOR_PAIR(DEFAULT_COLOR_PAIR));
    for (int i = 0; i < MAP_SIZE; i++) {
        int y = i / MAP_WIDTH;
        int x = i % MAP_WIDTH;

        int tile = get_seen_mapch(y, x);

        /* tiles we have seen get drawn slightly foggy */
        /* while tiles we are currently in the range of */
        /* we see clearly */
        if (tile & UNSEEN) {
            /* don't draw unseen tiles */
            if ((tile ^ UNSEEN) == ' ') continue;

            attrset(COLOR_PAIR(DEFAULT_COLOR_PAIR));
            mvaddch(y, x, tile ^ UNSEEN);
            set_seen_mapch(y, x, tile ^ UNSEEN);
        } else if (tile & SEEN) {
            attrset(COLOR_PAIR(DEFAULT_COLOR_PAIR) | A_BOLD);
            mvaddch(y, x, tile ^ SEEN);
            set_seen_mapch(y, x, tile ^ SEEN);
        }
    }
}

void reveal_partial_map(int py, int px, float fov) {
    for (int i = 0; i < MAP_SIZE; i++) {
        int y = i / MAP_WIDTH;
        int x = i % MAP_WIDTH;

        float dist = distance(y, x, py, px);

        if (dist < fov) {
            set_seen_mapch(y, x, get_mapch(y, x) | SEEN);
        } else if (dist < fov + 2) {
            set_seen_mapch(y, x, get_seen_mapch(y, x) | UNSEEN);
        }
    }
}

static void init_maps(void) {
    map = calloc(MAP_SIZE, sizeof(int));
    seen_map = calloc(MAP_SIZE, sizeof(int));

    for (int i = 0; i < MAP_SIZE; i++) {
        map[i] = ' ';
        seen_map[i] = ' ';
    }
}

static section_t make_section(int y, int x, int h, int w) {
    return (section_t){
        .y = y,
        .x = x,
        .h = h,
        .w = w,
        .active = false,
    };
}

static section_t append_section(section_t s) {
    sections[section_ptr].y = s.y;
    sections[section_ptr].x = s.x;
    sections[section_ptr].h = s.h;
    sections[section_ptr].w = s.w;
    sections[section_ptr].active = true;

    section_ptr++;
    if (section_ptr == MAX_SECTIONS) {
        NC_ABORT("no space for sections\n");
    }

    return sections[section_ptr];
}

static void generate_sections_recursive(section_t section) {
    /* BSP algorithm */

    section_t section1, section2;
    int split_horiz = rand() % 2;

    /* end if sections is small enough */
    if (section.h <= SECTION_MIN_H || section.w <= SECTION_MIN_W) {
        append_section(section);
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

    if (split_horiz) {
        int split_point = random_i(7, section.h - 7);
        section1 = make_section(section.y, section.x, split_point, section.w);
        section2 = make_section(section.y + split_point, section.x,
                                section.h - split_point, section.w);
    } else {
        int split_point = random_i(7, section.w - 7);
        section1 = make_section(section.y, section.x, section.h, split_point);
        section2 = make_section(section.y, section.x + split_point, section.h,
                                section.w - split_point);
    }

    generate_sections_recursive(section1);
    generate_sections_recursive(section2);
}

room_t append_room(int y, int x, int h, int w) {
    rooms[room_ptr].y = y;
    rooms[room_ptr].x = x;
    rooms[room_ptr].h = h;
    rooms[room_ptr].w = w;
    rooms[room_ptr].active = true;

    room_ptr++;
    if (room_ptr > MAX_ROOMS) {
        NC_ABORT("no space for room\n");
    }

    return rooms[room_ptr - 1];
}

static void create_rooms(void) {
    for (int i = 0; i < section_ptr; i++) {
        int y1, x1, y2, x2;

        /* clang-format off */
        do {
            y1 = random_i(sections[i].y + 1, sections[i].y + sections[i].h / 2 - 2);
            x1 = random_i(sections[i].x + 1, sections[i].x + sections[i].w / 2 - 2);
            y2 = random_i(sections[i].y + sections[i].h / 2 + 2, sections[i].y + sections[i].h - 1);
            x2 = random_i(sections[i].x + sections[i].w / 2 + 2, sections[i].x + sections[i].w - 1);
        } while (y2 - y1 < 4 || x2 - x1 < 4);
        /* clang-format on */

        int h = y2 - y1;
        int w = x2 - x1;
        append_room(y1, x1, h, w);

        /* draw room */
        /* sides (right and left) */
        for (int y = y1; y < y2; y++) {
            set_mapch(y, x1 - 1, '|');
            set_mapch(y, x2, '|');
        }

        /* top and bottom */
        for (int x = x1 - 1; x < x2 + 1; x++) {
            set_mapch(y1 - 1, x, '-');
            set_mapch(y2, x, '-');
        }

        /* fill with dungeon floor */
        for (int y = y1; y < y2; y++) {
            for (int x = x1; x < x2; x++) {
                set_mapch(y, x, '.');
                set_mapch(y, x, '.');
            }
        }
    }
}

static void make_corridors(void) {
    for (int i = 0; i < room_ptr; i++) {
        int cy1 = rooms[i].y + rooms[i].h / 2;
        int cx1 = rooms[i].x + rooms[i].w / 2;

        int j = i + (i + 1 == room_ptr ? 0 : 1);

        int cy2 = rooms[j].y + rooms[j].h / 2;
        int cx2 = rooms[j].x + rooms[j].w / 2;

        while (cy1 != cy2) {
            int c = '.';
            if (get_mapch(cy1, cx1) != '.') {
                c = '#';
            }

            set_mapch(cy1, cx1, c);
            cy1 += (cy1 < cy2) ? 1 : -1;
        }

        while (cx1 != cx2) {
            int c = '.';
            if (get_mapch(cy1, cx1) != '.') {
                c = '#';
            }

            set_mapch(cy1, cx1, c);
            cx1 += (cx1 < cx2) ? 1 : -1;
        }
    }
}

void generate_map(void) {
    section_t map = make_section(0, 0, MAP_HEIGHT, MAP_WIDTH);
    init_maps();
    generate_sections_recursive(map);
    create_rooms();
    make_corridors();
}
