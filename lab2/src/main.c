#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "sort.h"
#include "utils.h"

pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char* argv[]) {
    long max_threads = 1;
    int array_size = 1000;

    if (argc >= 2) {
        max_threads = atoi(argv[1]);
        if (max_threads <= 0) {
            const char msg[] = "Error: invalid count of threads provided";
            write(STDERR_FILENO, msg, sizeof(msg));
            return 1;
        }
    }

    if (argc >= 3) {
        array_size = atoi(argv[2]);
        if (array_size <= 0) {
            const char msg[] = "Error: invalid array size provided";
            write(STDERR_FILENO, msg, sizeof(msg));
            return 1;
        }
    }

    int* arr = (int*)malloc(array_size * sizeof(int));
    if (!arr) {
        const char msg[] = "Error: cant allocate memory";
        write(STDERR_FILENO, msg, sizeof(msg));
        return 1;
    }

    generate_random_array(arr, array_size);
    long active_threads = 1;

    sort_args_t args = {.array = arr,
                        .low = 0,
                        .high = array_size - 1,
                        .max_threads = max_threads,
                        .mutex = &thread_mutex,
                        .active_threads = &active_threads};

    struct timespec start;
    struct timespec end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    parallel_quicksort(&args);

    clock_gettime(CLOCK_MONOTONIC, &end);

    int is_sorted = 1;
    for (int i = 1; i < array_size; i++) {
        if (arr[i] < arr[i - 1]) {
            is_sorted = 0;
            break;
        }
    }
    free(arr);
    if (!is_sorted) {
        const char msg[] = "Error: array not sorted";
        write(STDERR_FILENO, msg, sizeof(msg));
        return 1;
    }

    double time_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                     (end.tv_nsec - start.tv_nsec) / 1000000.0;
    {
        char buf[1024];
        int cnt =
            snprintf(buf, sizeof(buf), "%ld | %.2f ms\n", max_threads, time_ms);
        write(STDOUT_FILENO, buf, cnt);
    }

    return 0;
}
