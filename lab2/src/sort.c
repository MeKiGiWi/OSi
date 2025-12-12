#include "../include/sort.h"

#include <pthread.h>
#include <stdlib.h>

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }

    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return (i + 1);
}

void sequential_quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        sequential_quicksort(arr, low, pi - 1);
        sequential_quicksort(arr, pi + 1, high);
    }
}

void* parallel_quicksort(void* args) {
    sort_args_t* data = (sort_args_t*)args;

    if (data->low < data->high) {
        pthread_mutex_lock(data->mutex);
        if (*data->active_threads >= data->max_threads ||
            (data->high - data->low) < 100) {
            pthread_mutex_unlock(data->mutex);
            sequential_quicksort(data->array, data->low, data->high);
        } else {
            pthread_mutex_unlock(data->mutex);
            int pi = partition(data->array, data->low, data->high);

            pthread_t left_thread, right_thread;

            sort_args_t left_args = {.array = data->array,
                                     .low = data->low,
                                     .high = pi - 1,
                                     .max_threads = data->max_threads,
                                     .mutex = data->mutex,
                                     .active_threads = data->active_threads};

            sort_args_t right_args = {.array = data->array,
                                      .low = pi + 1,
                                      .high = data->high,
                                      .max_threads = data->max_threads,
                                      .mutex = data->mutex,
                                      .active_threads = data->active_threads};

            int left_created = 0, right_created = 0;

            if (left_args.low < left_args.high) {
                pthread_mutex_lock(data->mutex);
                (*data->active_threads)++;
                if (!pthread_create(&left_thread, NULL, parallel_quicksort,
                                    &left_args)) {
                    left_created = 1;

                } else {
                    sequential_quicksort(left_args.array, left_args.low,
                                         left_args.high);
                    (*data->active_threads)--;
                }
                pthread_mutex_unlock(data->mutex);
            }

            if (right_args.low < right_args.high) {
                pthread_mutex_lock(data->mutex);
                (*data->active_threads)++;

                if (!pthread_create(&right_thread, NULL, parallel_quicksort,
                                    &right_args)) {
                    right_created = 1;
                } else {
                    sequential_quicksort(right_args.array, right_args.low,
                                         right_args.high);
                    (*data->active_threads)--;
                }
                pthread_mutex_unlock(data->mutex);
            }

            if (left_created) {
                pthread_join(left_thread, NULL);
                pthread_mutex_lock(data->mutex);
                (*data->active_threads)--;
                pthread_mutex_unlock(data->mutex);
            } else if (left_args.low < left_args.high) {
                sequential_quicksort(left_args.array, left_args.low,
                                     left_args.high);
            }

            if (right_created) {
                pthread_join(right_thread, NULL);
                pthread_mutex_lock(data->mutex);
                (*data->active_threads)--;
                pthread_mutex_unlock(data->mutex);
            } else if (right_args.low < right_args.high) {
                sequential_quicksort(right_args.array, right_args.low,
                                     right_args.high);
            }
        }
    }

    return NULL;
}
