#ifndef MONSTERS_H
#define MONSTERS_H

#include <stdbool.h>

#define MONSTERS_FOV 4.5
#define MAX_MONSTERS 100

typedef struct {
    int y, x;
    char *name;
    char symbol;
	int exp_gain;

    int health;
    int damage;
    float speed;

    bool active;
} monster_t;

monster_t make_monster(int y, int x, char symbol);
void append_monster(monster_t monster);
monster_t *find_monster(int y, int x);

void create_monsters(void);
void update_monsters(int py, int px, int *health, int prot);
void draw_monsters(int py, int px, float fov);

#endif /* MONSTERS_H */
