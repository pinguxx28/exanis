#ifndef COLORS_H
#define COLORS_H

enum {
    DEFAULT_COLOR_PAIR = 1,
    INVERSE_DEFAULT_COLOR_PAIR,
    PLAYER_COLOR_PAIR,
    MONEY_COLOR_PAIR,
    MONSTERS_COLOR_PAIR,
    ORC_COLOR_PAIR,
    WEAPON_COLOR_PAIR,
	ARMOR_COLOR_PAIR,
};

void ensure_colors(void);
void init_color_pairs(void);

#endif /* COLORS_H */
