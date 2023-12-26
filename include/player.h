#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_FOV 1000

typedef struct {
    int y, x;
    int money;
    int health;
    int damage;
    int speed;
} player_t;

player_t *init_player(void);
void draw_player(player_t);
void clear_player(player_t);
void move_player(player_t *, int);
void draw_player_stats(player_t player);

#endif /* PLAYER_H */
