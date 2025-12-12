#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char buf[4096];
    ssize_t bytes_read;
    int32_t written;
    int32_t left;
    int32_t right;

    while ((bytes_read = read(STDIN_FILENO, buf, sizeof(buf)))) {
        if (bytes_read < 0) {
            const char msg[] = "error: failed to read from stdin\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            exit(EXIT_FAILURE);
        }
        left = 0;
        right = bytes_read - 2;
        while (left < right) {
            char tmp = buf[left];
            buf[left] = buf[right];
            buf[right] = tmp;
            ++left;
            --right;
        }
        written = write(STDOUT_FILENO, buf, bytes_read);
        if (written != bytes_read) {
            const char msg[] = "error: failed to echo\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}
