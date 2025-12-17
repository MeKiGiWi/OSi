#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "math_lib.h"
#include "sort_lib.h"

int main(void) {
    float a = 1.0f;
    float dx = 0.001f;

    char buffer[256];
    int len = snprintf(buffer, sizeof(buffer), "Testing math functions:\n");
    write(STDOUT_FILENO, buffer, len);

    len = snprintf(buffer, sizeof(buffer),
                   "cos_derivative_method1(%.3f, %.3f) = %.6f\n", a, dx,
                   cos_derivative_method1(a, dx));
    write(STDOUT_FILENO, buffer, len);

    len = snprintf(buffer, sizeof(buffer),
                   "cos_derivative_method2(%.3f, %.3f) = %.6f\n", a, dx,
                   cos_derivative_method2(a, dx));
    write(STDOUT_FILENO, buffer, len);

    size_t array_size = 10;
    int* arr = malloc(array_size * sizeof(int));

    if (!arr) {
        const char msg[] = "Error: cant allocate memory";
        write(STDERR_FILENO, msg, sizeof(msg));
        return EXIT_FAILURE;
    }

    srand((unsigned int)time(NULL));

    len = snprintf(buffer, sizeof(buffer), "\nOriginal array: ");
    write(STDOUT_FILENO, buffer, len);

    for (size_t i = 0; i < array_size; ++i) {
        arr[i] = rand() % 101;
        len = snprintf(buffer, sizeof(buffer), "%d ", arr[i]);
        write(STDOUT_FILENO, buffer, len);
    }

    int* bubble_sorted = sort_bubble(arr, array_size);

    len = snprintf(buffer, sizeof(buffer), "\nBubble sorted array: ");
    write(STDOUT_FILENO, buffer, len);

    for (size_t i = 0; i < array_size; ++i) {
        len = snprintf(buffer, sizeof(buffer), "%d ", bubble_sorted[i]);
        write(STDOUT_FILENO, buffer, len);
    }

    srand((unsigned int)time(NULL));
    for (size_t i = 0; i < array_size; ++i) {
        arr[i] = rand() % 101;
    }

    int* quick_sorted = sort_quicksort(arr, array_size);

    len = snprintf(buffer, sizeof(buffer), "\nQuick sorted array: ");
    write(STDOUT_FILENO, buffer, len);

    for (size_t i = 0; i < array_size; ++i) {
        len = snprintf(buffer, sizeof(buffer), "%d ", quick_sorted[i]);
        write(STDOUT_FILENO, buffer, len);
    }

    free(arr);

    return EXIT_SUCCESS;
}
