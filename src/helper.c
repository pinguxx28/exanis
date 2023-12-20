#include "../include/helper.h"

#include <math.h>
#include <stdlib.h>

int random_i(int low, int high) {
    if (low == high) {
        return low;
    }

    return low + (rand() % (high - low));
}
float random_f(float low, float high) {
    if (low == high) {
        return low;
    }

    return low + ((float)rand() / (float)RAND_MAX) * (high - low);
}

float distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
