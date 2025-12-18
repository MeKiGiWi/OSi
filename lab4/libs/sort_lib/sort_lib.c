#include "sort_lib.h"

#include <stddef.h>

int* sort_bubble(int* array, size_t n) {
    if (n <= 1 || !array) return array;

    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    return array;
}

static void quicksort_recursive(int* array, int low, int high) {
    if (low >= high) return;

    int pivot = array[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (array[j] <= pivot) {
            i++;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }

    i++;
    int temp = array[i];
    array[i] = array[high];
    array[high] = temp;

    quicksort_recursive(array, low, i - 1);
    quicksort_recursive(array, i + 1, high);
}

int* sort_quicksort(int* array, size_t n) {
    if (n <= 1 || !array) return array;

    quicksort_recursive(array, 0, (int)(n - 1));

    return array;
}
