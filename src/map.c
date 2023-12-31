#include "../include/map.h"

#include "../include/colors.h"
#include "../include/debug.h"
#include "../include/helper.h"
#include <ncurses.h>

#define SECTION_MIN_W 13
#define SECTION_MIN_H 13

#define CHECK_BOUNDS(y, x, fname)                                              \
    do {                                                                       \
        if ((y) < 0 || (y) >= MAP_HEIGHT || (x) < 0 || (x) >= MAP_WIDTH) {     \
			*(int*)0 = 0; \
            NC_ABORT("out of bounds\ny: %d, x: %d, inside of %s\n", (y), (x),  \
                     (fname));                                                 \
        }                                                                      \
    } while (0)

int *map;
int *seen_map;

int num_sections = 0;
int num_rooms = 0;
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

void draw_map(void) {
    attrset(COLOR_PAIR(DEFAULT_COLOR_PAIR));

    for (int i = 0; i < MAP_SIZE; i++) {
        int y = i / MAP_WIDTH;
        int x = i % MAP_WIDTH;

        int tile = get_seen_mapch(y, x);
		attrset(COLOR_PAIR(DEFAULT_COLOR_PAIR));

        /* tiles we have seen get drawn slightly foggy 
         * while tiles we are currently in the range of
         * we see clearly */
        if (tile & UNSEEN) {
            mvaddch(y, x, tile ^ UNSEEN);
            set_seen_mapch(y, x, tile ^ UNSEEN);
        } else if (tile & SEEN) {
            attron(A_BOLD);
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
    sections[num_sections].y = s.y;
    sections[num_sections].x = s.x;
    sections[num_sections].h = s.h;
    sections[num_sections].w = s.w;
    sections[num_sections].active = true;

    num_sections++;
    if (num_sections == MAX_SECTIONS) {
        NC_ABORT("no space for sections\n");
    }

    return sections[num_sections];
}

static void create_sections_recursive(section_t section) {
    /* BSP algorithm */

    /* end recursion if section is small enough */
    if (section.h <= SECTION_MIN_H || section.w <= SECTION_MIN_W) {
        append_section(section);
        return;
    }

    /* force split in the desired direction */
    float size_ratio = 2;
    int split_horiz = rand() % 2;
    if (section.w * size_ratio > section.h) {
        split_horiz = 0;
    }
    if (section.h * size_ratio > section.w) {
        split_horiz = 1;
    }

    section_t section1, section2;
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

    create_sections_recursive(section1);
    create_sections_recursive(section2);
}

static room_t append_room(int y, int x, int h, int w) {
    rooms[num_rooms].y = y;
    rooms[num_rooms].x = x;
    rooms[num_rooms].h = h;
    rooms[num_rooms].w = w;
    rooms[num_rooms].active = true;

    num_rooms++;
    if (num_rooms > MAX_ROOMS) {
        NC_ABORT("no space for room\n");
    }

    return rooms[num_rooms - 1];
}

static void create_rooms(void) {
    for (int i = 0; i < num_sections; i++) {
		int y = sections[i].y;
		int x = sections[i].x;
		int h = sections[i].h;
		int w = sections[i].w;

		int centre_y = y + h / 2;
		int centre_x = x + w / 2;

		const int edge_padding = 1;
		const int centre_padding = 2;

		const int min_w = 4;
		const int min_h = 4; 

        int y1, x1, y2, x2;

        do {
            y1 = random_i(y + edge_padding, centre_y - centre_padding);
            x1 = random_i(x + edge_padding, centre_x - centre_padding);
            y2 = random_i(centre_y + centre_padding, y + h - edge_padding);
            x2 = random_i(centre_x + centre_padding, x + w - edge_padding);
        } while (y2 - y1 < min_h || x2 - x1 < min_w);

		w = x2 - x1;
		h = y2 - y1;

        append_room(y1, x1, h, w);

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

static void create_corridors(void) {
    for (int i = 0; i < num_rooms; i++) {
        int centre_y1 = rooms[i].y + rooms[i].h / 2;
        int centre_x1 = rooms[i].x + rooms[i].w / 2;

        int next_room = i + (i + 1 == num_rooms ? 0 : 1);

        int centre_y2 = rooms[next_room].y + rooms[next_room].h / 2;
        int centre_x2 = rooms[next_room].x + rooms[next_room].w / 2;

        while (centre_y1 != centre_y2) {
            int c = get_mapch(centre_y1, centre_x1) == '.' ? '.' : '#';

            set_mapch(centre_y1, centre_x1, c);
            centre_y1 += (centre_y1 < centre_y2) ? 1 : -1;
        }

        while (centre_x1 != centre_x2) {
            int c = get_mapch(centre_y1, centre_x1) == '.' ? '.' : '#';

            set_mapch(centre_y1, centre_x1, c);
            centre_x1 += (centre_x1 < centre_x2) ? 1 : -1;
        }
    }
}

static void create_staircase(void) {
    int n = random_i(0, num_rooms);

    int y = random_i(rooms[n].y, rooms[n].y + rooms[n].h);
    int x = random_i(rooms[n].x, rooms[n].x + rooms[n].w);

    set_mapch(y, x, '>');
}

void create_map(void) {
    section_t map = make_section(0, 0, MAP_HEIGHT, MAP_WIDTH);
    init_maps();
    create_sections_recursive(map);
    create_rooms();
    create_corridors();
    create_staircase();
}
