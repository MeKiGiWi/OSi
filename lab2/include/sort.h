#pragma once
#include <pthread.h>

typedef struct {
    int* array;
    int low;
    int high;
    long max_threads;
    pthread_mutex_t* mutex;
    long* active_threads;
} sort_args_t;

int partition(int arr[], int low, int high);
void sequential_quicksort(int arr[], int low, int high);
void* parallel_quicksort(void* args);
