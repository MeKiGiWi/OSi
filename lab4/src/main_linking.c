#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "math_lib.h"
#include "sort_lib.h"

int main(void) {
    char input_buffer[256];

    char buffer[256];
    int len;

    len = snprintf(buffer, sizeof(buffer),
                   " 1 <arg1> <arg2> - Calculate cosine derivative (takes 2 "
                   "args: value, dx)\n");
    write(STDOUT_FILENO, buffer, len);

    len = snprintf(buffer, sizeof(buffer),
                   " 2 <arg1> <arg2> ... <argN> - Sort array (provide at least "
                   "1 number)\n");
    write(STDOUT_FILENO, buffer, len);

    len = snprintf(buffer, sizeof(buffer), "  Enter command: ");
    write(STDOUT_FILENO, buffer, len);

    ssize_t bytes_read;
    while ((bytes_read = read(STDIN_FILENO, input_buffer,
                              sizeof(input_buffer) - 1)) > 0) {
        input_buffer[bytes_read] = 0;
        char* newline = strchr(input_buffer, '\n');
        if (newline) {
            *newline = 0;
        }

        char* token = strtok(input_buffer, " \t\n");
        if (!token) {
            len = snprintf(buffer, sizeof(buffer), "Enter command: ");
            write(STDOUT_FILENO, buffer, len);
            continue;
        }

        if (!strcmp(token, "1")) {
            char* arg1_str = strtok(NULL, " \t\n");
            char* arg2_str = strtok(NULL, " \t\n");

            if (!arg1_str || !arg2_str) {
                len = snprintf(
                    buffer, sizeof(buffer),
                    "Error: Command 1 requires 2 arguments (value, dx)\n");
                write(STDERR_FILENO, buffer, len);
                return EXIT_FAILURE;
            }

            float a = (float)atof(arg1_str);
            float dx = (float)atof(arg2_str);

            float result1 = cos_derivative_method1(a, dx);
            float result2 = cos_derivative_method2(a, dx);

            len = snprintf(buffer, sizeof(buffer),
                           "cos_derivative_method1(%.3f, %.3f) = %.6f\n", a, dx,
                           result1);
            write(STDOUT_FILENO, buffer, len);

            len = snprintf(buffer, sizeof(buffer),
                           "cos_derivative_method2(%.3f, %.3f) = %.6f\n", a, dx,
                           result2);
            write(STDOUT_FILENO, buffer, len);

        } else if (!strcmp(token, "2")) {
            int temp_array[100];
            int count = 0;

            char* arg;
            while ((arg = strtok(NULL, " \t\n")) && count < 100) {
                temp_array[count] = atoi(arg);
                count++;
            }

            if (!count) {
                len = snprintf(buffer, sizeof(buffer),
                               "Error: Command 2 requires at least 1 argument "
                               "for sorting\n");
                write(STDERR_FILENO, buffer, len);
                return EXIT_FAILURE;
            }

            len = snprintf(buffer, sizeof(buffer), "Original array: ");
            write(STDOUT_FILENO, buffer, len);

            for (int i = 0; i < count; i++) {
                len = snprintf(buffer, sizeof(buffer), "%d ", temp_array[i]);
                write(STDOUT_FILENO, buffer, len);
            }

            int* temp_array_bubble = malloc(count * sizeof(int));
            int* temp_array_quick = malloc(count * sizeof(int));

            for (int i = 0; i < count; i++) {
                temp_array_bubble[i] = temp_array[i];
                temp_array_quick[i] = temp_array[i];
            }

            sort_bubble(temp_array_bubble, count);
            len =
                snprintf(buffer, sizeof(buffer), "\nSorted with bubble sort: ");
            write(STDOUT_FILENO, buffer, len);

            for (int i = 0; i < count; i++) {
                len = snprintf(buffer, sizeof(buffer), "%d ",
                               temp_array_bubble[i]);
                write(STDOUT_FILENO, buffer, len);
            }

            sort_quicksort(temp_array_quick, count);
            len = snprintf(buffer, sizeof(buffer), "\nSorted with quicksort: ");
            write(STDOUT_FILENO, buffer, len);

            for (int i = 0; i < count; i++) {
                len = snprintf(buffer, sizeof(buffer), "%d ",
                               temp_array_quick[i]);
                write(STDOUT_FILENO, buffer, len);
            }

            len = snprintf(buffer, sizeof(buffer), "\n");
            write(STDOUT_FILENO, buffer, len);

            free(temp_array_bubble);
            free(temp_array_quick);

        } else if (!strcmp(token, "exit")) {
            return EXIT_SUCCESS;
        } else {
            len = snprintf(buffer, sizeof(buffer), "Unknown command");
            write(STDERR_FILENO, buffer, len);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
