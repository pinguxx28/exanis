#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_FOV 2.5

#include "weapon.h"
#include "armor.h"

typedef struct {
    int y, x;

	bool godmode;

    int money;
	int experience;
	int dungeon_level;

    int health;
    int damage;
    int speed;
	int protection;

	/* effects */
	int timer;
	bool regen;

    weapon_t weapon;
	armor_t armor;
} player_t;

player_t *init_player(bool godmode);
void clear_player(player_t);
void update_player(player_t *, int c);
void draw_player(player_t);
void draw_player_stats(player_t player);

#endif /* PLAYER_H */
