#include "utils.h"

#include <stdlib.h>
#include <time.h>

void generate_random_array(int* arr, int size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 1000;
    }
}
