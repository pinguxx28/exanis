#ifndef MONSTERS_H
#define MONSTERS_H

#include <stdbool.h>

#define N_MONSTERS 1000
typedef struct {
    int y, x;
    char symbol;
    int health;
    int damage;
    int speed;
    bool active;
} monster_t;

monster_t *find_monster(int y, int x);
monster_t make_monster(int y, int x, char symbol, int health, int damage,
                       int speed);
void append_monster(monster_t monster);

void init_monsters(void);
void draw_monsters(int py, int px, float fov);

#endif /* MONSTERS_H */
