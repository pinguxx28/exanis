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

float distance(int y1, int x1, int y2, int x2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

float minf(float a, float b) {
    return a < b ? a : b;
}

float maxf(float a, float b) {
    return a > b ? a : b;
}
