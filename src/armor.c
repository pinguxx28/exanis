#include "armor.h"

#include <stdlib.h>
#include <string.h>

#include "debug.h"

armor_t make_armor(armor_type type) {
    const char *armor_names[armor_TYPE_N] =
		{ "fist", "sword" };
    armor_t armor;

    armor.type = type;
    armor.name = malloc(strlen(armor_names[type]) + 1);
    strcpy(armor.name, armor_names[type]);

	/* values can be hard coded here because they are not used anywhere else */
    switch (type) {
		case NONE:
			armor.protection = 0;
			armor.durability = 0;
			break;
        case MAIL:
            armor.protection = 5;
            armor.durability = 25;
            break;
        default:
			NC_ABORT("Unhandled armor type\nType=%d\n", type);
    }

    return armor;
}
