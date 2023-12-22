#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_FOV 2.5

typedef struct {
    int x, y;
} player_t;

player_t *init_player(void);
void draw_player(player_t *);
void clear_player(player_t *);
void move_player(player_t *, int);

#endif /* PLAYER_H */
