#ifndef WEAPON_H
#define WEAPON_H

#include <stdbool.h>

typedef enum {
    FIST,
    SWORD,
    CLUB,
    MACHETE,
	WEAPON_TYPE_N
} weapon_type;

typedef struct {
    char *name;
    int damage;
    int durability;
    float speed;
    bool two_handed;
    weapon_type type;
} weapon_t;

weapon_t make_weapon(weapon_type type);

#endif /* WEAPON_H */
