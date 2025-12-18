#include <ctype.h>
#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define FIRST 0
#define SECOND 1

typedef enum { CODE_STATUS_OK = 0, CODE_STATUS_ER_DLOPEN = 1 } CodeStatus;

typedef float (*cos_derivative_func_p)(float, float);
typedef int* (*sort_func_p)(int*, size_t);

CodeStatus command_0(const char** LIB_NAMES, void** library, int* current_lib,
                     cos_derivative_func_p* cos_derivative, sort_func_p* sort) {
    dlclose(*library);
    switch (*current_lib) {
        case FIRST:
            *current_lib = SECOND;
            break;
        case SECOND:
            *current_lib = FIRST;
            break;
    }

    char buffer[BUFFER_SIZE];

    *library = dlopen(LIB_NAMES[*current_lib], RTLD_LAZY);
    if (!(*library)) {
        int len =
            snprintf(buffer, BUFFER_SIZE, "Error switching: %s\n", dlerror());
        write(STDERR_FILENO, buffer, len);
        return CODE_STATUS_ER_DLOPEN;
    }

    *cos_derivative = NULL;
    *sort = NULL;

    if (*current_lib == FIRST) {
        *cos_derivative =
            (cos_derivative_func_p)dlsym(*library, "cos_derivative_method1");
        if (!cos_derivative) {
            const char msg[] =
                "warning: failed to find cos_derivative function "
                "implementation\n";
            write(STDERR_FILENO, msg, sizeof(msg));
        }
    } else {
        *sort = (sort_func_p)dlsym(*library, "sort_quicksort");
        if (!sort) {
            const char msg[] =
                "warning: failed to find sort function implementation\n";
            write(STDERR_FILENO, msg, sizeof(msg));
        }
    }

    {
        int len = snprintf(buffer, BUFFER_SIZE, "Switched to library: %s\n",
                           LIB_NAMES[*current_lib]);
        write(STDOUT_FILENO, buffer, len);
    }

    return CODE_STATUS_OK;
}

void command_1(cos_derivative_func_p cos_derivative) {
    if (!cos_derivative) {
        const char* msg =
            "Cos derivative function not available in current library\n";
        write(STDERR_FILENO, msg, strlen(msg));
        return;
    }

    char* arg1 = strtok(NULL, " \t\n");
    if (!arg1) {
        const char* msg = "Error: too few args for cos\n";
        write(STDERR_FILENO, msg, strlen(msg));
        return;
    }
    char* arg2 = strtok(NULL, " \t\n");
    if (!arg2) {
        const char* msg = "Error: too few args for cos\n";
        write(STDERR_FILENO, msg, strlen(msg));
        return;
    }
    int len = 0;
    char buffer[BUFFER_SIZE];

    if (arg1 && arg2) {
        float res = cos_derivative((float)atof(arg1), (float)atof(arg2));
        len =
            snprintf(buffer, BUFFER_SIZE, "Cos derivative result: %.5f\n", res);

        write(STDOUT_FILENO, buffer, len);
    }
}

void command_2(sort_func_p sort) {
    if (!sort) {
        const char* msg = "Sort function not available in current library\n";
        write(STDERR_FILENO, msg, strlen(msg));
        return;
    }

    char* size_str = strtok(NULL, " \t\n");
    if (!size_str) return;

    int size = atoi(size_str);
    if (size <= 0) {
        const char* msg = "Invalid array size\n";
        write(STDERR_FILENO, msg, strlen(msg));
        return;
    }

    int* array = malloc(size * sizeof(int));
    if (!array) {
        const char* message = "Unable to malloc memory for int* array\n";
        write(STDERR_FILENO, message, strlen(message));
        return;
    }

    for (int i = 0; i < size; ++i) {
        char* value = strtok(NULL, " \t\n");
        if (value)
            array[i] = atoi(value);
        else
            array[i] = 0;
    }

    sort(array, size);

    {
        const char* message = "Sorted array: ";
        write(STDOUT_FILENO, message, strlen(message));
    }

    char buffer[BUFFER_SIZE];
    for (int i = 0; i < size; ++i) {
        int len = snprintf(buffer, BUFFER_SIZE, "%d ", array[i]);
        write(STDOUT_FILENO, buffer, len);
    }

    write(STDOUT_FILENO, "\n", 1);
    free(array);
}

int main(void) {
    const char* LIB_NAMES[] = {"./out/Debug/libs/libmath_lib.so",
                               "./out/Debug/libs/libsort_lib.so"};
    int current_lib = FIRST;

    cos_derivative_func_p cos_derivative = NULL;
    sort_func_p sort = NULL;

    char buffer[BUFFER_SIZE];
    int len;
    len = snprintf(buffer, sizeof(buffer),
                   " 1 <arg1> <arg2> - Calculate cosine derivative (takes 2 "
                   "args: value, dx)\n");
    write(STDOUT_FILENO, buffer, len);

    len = snprintf(buffer, sizeof(buffer),
                   " 2 <arg1> <arg2> ... <argN> - Sort array (provide at least "
                   "2 numbers)\n");
    write(STDOUT_FILENO, buffer, len);

    void* library = dlopen(LIB_NAMES[current_lib], RTLD_LAZY);
    if (!library) {
        len = snprintf(buffer, BUFFER_SIZE, "Error loading library: %s\n",
                       dlerror());
        write(STDERR_FILENO, buffer, len);
        return CODE_STATUS_ER_DLOPEN;
    }

    if (current_lib == FIRST) {
        cos_derivative =
            (cos_derivative_func_p)dlsym(library, "cos_derivative_method1");
        if (!cos_derivative) {
            const char msg[] =
                "Error: failed to find cos_derivative function "
                "implementation\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            return EXIT_FAILURE;
        }
        sort = NULL;
    } else {
        sort = (sort_func_p)dlsym(library, "sort_quicksort");
        if (!sort) {
            const char msg[] =
                "Error: failed to find sort function implementation\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            return EXIT_FAILURE;
        }
        cos_derivative = NULL;
    }

    ssize_t bytes_read;
    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[bytes_read] = 0;

        char* token = strtok(buffer, " \t\n");
        if (!token) return EXIT_SUCCESS;

        if (strlen(token) != 1 || !isdigit(token[0])) {
            const char msg[] = "Error: command must be degit\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            return EXIT_FAILURE;
        }
        int cmd = atoi(token);
        switch (cmd) {
            case 0: {
                int result = command_0(LIB_NAMES, &library, &current_lib,
                                       &cos_derivative, &sort);
                if (result != CODE_STATUS_OK) return EXIT_FAILURE;
                break;
            }
            case 1: {
                command_1(cos_derivative);
                break;
            }
            case 2: {
                command_2(sort);
                break;
            }
        }

        write(STDOUT_FILENO, "> ", 2);
    }

    if (library) dlclose(library);
    return EXIT_SUCCESS;
}
