#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    const char CHILD_PROGRAM_NAME[] = "child";
    const char OUTPUT_FOLDER_NAME[] = "output_files";
    int pipefd1[2];
    int pipefd2[2];
    pid_t child1;
    pid_t child2;
    char main_binary_path[1024];
    char output_path[1024];
    {
        char progpath[1024];
        ssize_t len =
            readlink("/proc/self/exe", progpath, sizeof(progpath) - 1);
        if (len == -1) {
            const char msg[] = "error: failed to read full program path\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            exit(EXIT_FAILURE);
        }

        while (progpath[len] != '/') --len;

        progpath[len] = '\0';
        snprintf(main_binary_path, sizeof(main_binary_path) - 1, "%s/%s",
                 progpath, CHILD_PROGRAM_NAME);

        while (progpath[len] != '/') --len;
        progpath[len] = '\0';
        snprintf(output_path, sizeof(output_path) - 1, "%s/%s", progpath,
                 OUTPUT_FOLDER_NAME);
    }

    if (pipe(pipefd1) == -1) {
        const char msg[] = "error: failed create pipe\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd2) == -1) {
        const char msg[] = "error: failed create pipe\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }

    char buf[4096];
    ssize_t bytes_read;

    bytes_read = read(STDIN_FILENO, buf, sizeof(buf));
    if (bytes_read < 0) {
        const char msg[] = "error: failed to read from stdin\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    } else if (buf[0] == '\n' || bytes_read == 0) {
        const char msg[] = "error: invalid file name\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }

    char file_path[1024];
    buf[bytes_read - 1] = '\0';
    snprintf(file_path, sizeof(file_path) - 1, "%s/%s", output_path, buf);
    int32_t file1 = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (file1 == -1) {
        const char msg[] = "error: failed to open requested file\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }

    bytes_read = read(STDIN_FILENO, buf, sizeof(buf));
    if (bytes_read < 0) {
        const char msg[] = "error: failed to read from stdin\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    } else if (buf[0] == '\n') {
        const char msg[] = "error: invalid file name\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }

    buf[bytes_read - 1] = '\0';
    snprintf(file_path, sizeof(file_path) - 1, "%s/%s", output_path, buf);

    int32_t file2 = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (file2 == -1) {
        const char msg[] = "error: failed to open requested file\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }

    child1 = fork();
    if (child1 == -1) {
        const char msg[] = "error: failed to spawn new process\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }
    if (child1 == 0) {
        dup2(file1, STDOUT_FILENO);
        close(file1);

        close(pipefd1[1]);
        dup2(pipefd1[0], STDIN_FILENO);
        close(pipefd1[0]);
        close(pipefd2[0]);
        close(pipefd2[1]);

        char* const args[] = {"child", NULL};
        execv(main_binary_path, args);
        const char msg[] = "error: failed to execv\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }

    child2 = fork();
    if (child2 == -1) {
        const char msg[] = "error: failed to spawn new process\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }
    if (child2 == 0) {
        dup2(file2, STDOUT_FILENO);
        close(file2);

        close(pipefd2[1]);
        dup2(pipefd2[0], STDIN_FILENO);
        close(pipefd2[0]);
        close(pipefd1[0]);
        close(pipefd1[1]);

        char* const args[] = {"child", NULL};
        execv("./child", args);
        const char msg[] = "error: failed to execv\n";
        write(STDERR_FILENO, msg, sizeof(msg));
        exit(EXIT_FAILURE);
    }

    while (bytes_read = read(STDIN_FILENO, buf, sizeof(buf))) {
        if (bytes_read < 0) {
            const char msg[] = "error: failed to read from stdin\n";
            write(STDERR_FILENO, msg, sizeof(msg));
            exit(EXIT_FAILURE);
        } else if (buf[0] == '\n') {
            break;
        }
        if (bytes_read > 11) {
            write(pipefd2[1], buf, bytes_read);
        } else {
            write(pipefd1[1], buf, bytes_read);
        }
    }
    close(pipefd2[1]);
    close(pipefd1[1]);
    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);
    return 0;
}
