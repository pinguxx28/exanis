#ifndef COLORS_H
#define COLORS_H

enum {
    DEFAULT_COLOR_PAIR = 1,
    PLAYER_COLOR_PAIR,
    MONEY_COLOR_PAIR,
};

void ensure_colors(void);
void init_color_pairs(void);

#endif /* COLORS_H */
