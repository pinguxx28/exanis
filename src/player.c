#include "../include/player.h"

#include <ncurses.h>
#include <stdlib.h>

#include "../include/colors.h"

player_t *init_player(void) {
    player_t *player = calloc(1, sizeof(player_t));
    player->x = 0;
    player->y = 0;
    return player;
}

void draw_player(player_t *player) {
    attron(COLOR_PAIR(PLAYER_COLOR_PAIR));
    mvaddch(player->y, player->x, '@');
    attroff(COLOR_PAIR(PLAYER_COLOR_PAIR));
}

void clear_player(player_t *player) {
    /* clear the last player position */
    mvaddch(player->y, player->x, ' ');
}

void move_player(player_t *player, int c) {
    /* clang-format off */
    if (c == 'h') player->x--;
    else if (c == 'l') player->x++;
    else if (c == 'k') player->y--;
    else if (c == 'j') player->y++;
    else if (c == 'y') { player->y--; player->x--; }
    else if (c == 'u') { player->y--; player->x++; }
    else if (c == 'b') { player->y++; player->x--; }
    else if (c == 'n') { player->y++; player->x++; }
    /* clang-format on */
}
