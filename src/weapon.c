#include "../include/weapon.h"

#include <stdlib.h>
#include <string.h>

weapon_t make_weapon(weapon_type type) {
    const char *weapon_names[WEAPON_TYPE_N] = {
        "fist",           "sword",   "club",   "machete", "dagger",
        "spear",          "axe",     "mace",   "cleaver", "glavie",
        "scissor blades", "halberd", "katana", "hammer",  "rapier"};
    weapon_t weapon;

    weapon.name = malloc(24);
    strcpy(weapon.name, weapon_names[(int)type]);
    weapon.type = type;

    switch (type) {
        case SWORD:
            weapon.damage = 5;
            weapon.durability = 25;
            weapon.speed = 1.0 / 3.0;
            weapon.two_handed = true;
            break;
        default:
            weapon.damage = 1;
            weapon.durability = 1;
            weapon.speed = 1.0 / 1.0;
            weapon.two_handed = false;
            break;
    }

    return weapon;
}
