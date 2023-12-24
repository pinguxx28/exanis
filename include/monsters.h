#ifndef MONSTERS_H
#define MONSTERS_H

#include <stdbool.h>

#define MONSTERS_FOV 4.5

#define N_MONSTERS 1000
typedef struct {
    int y, x;
    char symbol;
    int health;
    int damage;
    float speed;
    bool active;
} monster_t;

monster_t *find_monster(int y, int x);
monster_t make_monster(int y, int x, char symbol, int health, int damage,
                       float speed);
void append_monster(monster_t monster);

void init_monsters(void);
void update_monsters(int py, int px, int *health);
void draw_monsters(int py, int px, float fov);

#endif /* MONSTERS_H */
