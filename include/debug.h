#ifndef DEBUG_H
#define DEBUG_H

/* make sure we have all libraries needed */
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define NC_ABORT(...)                                                          \
    do {                                                                       \
        endwin();                                                              \
        fprintf(stderr, __VA_ARGS__);                                          \
        exit(1);                                                               \
    } while (0)

#endif /* DEBUG_H */
