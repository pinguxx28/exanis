#include "weapon.h"

#include <stdlib.h>
#include <string.h>

#include "debug.h"

weapon_t make_weapon(weapon_type type) {
    const char *weapon_names[WEAPON_TYPE_N] =
		{ "fist", "sword", "club", "machete" };
    weapon_t weapon;

    weapon.type = type;
    weapon.name = malloc(strlen(weapon_names[type]) + 1);
    strcpy(weapon.name, weapon_names[type]);

	/* WARNING: Bad hard coded values */
    switch (type) {
		case FIST:
			weapon.damage = 2;
			weapon.durability = 100;
			weapon.speed = 4.0 / 5.0;
			break;
        case SWORD:
            weapon.damage = 5;
            weapon.durability = 25;
            weapon.speed = 1.0 / 3.0;
            break;
        default:
			NC_ABORT("Unhandled weapon type\nType=%d\n", type);
    }

    return weapon;
}
