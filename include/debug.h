#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

extern FILE *debug_file;
void open_debug_file(const char *filepath);

#endif /* DEBUG_H */
