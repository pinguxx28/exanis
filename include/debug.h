#ifndef DEBUG_H
#define DEBUG_H

/* ensures that we have all the functions required */
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define NC_ABORT(...)                                                          \
    do {                                                                       \
        endwin();                                                              \
        fprintf(stderr, __VA_ARGS__);                                          \
        exit(1);                                                               \
    } while (0)

#endif /* DEBUG_H */
