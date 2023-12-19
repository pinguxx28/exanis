#include "../include/helper.h"

#include <stdlib.h>

int random_i(int low, int high) { return low + (rand() % (high - low)); }
float random_f(float low, float high) {
    return low + ((float)rand() / (float)RAND_MAX) * (high - low);
}
