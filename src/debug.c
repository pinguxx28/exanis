#include "../include/debug.h"

#include <assert.h>

FILE *debug_file;

void open_debug_file(const char *filepath) {
    debug_file = fopen(filepath, "w");
    assert(debug_file != NULL);
}
