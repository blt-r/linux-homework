#include <pwd.h>
#include <sys/ucontext.h>
#include <unistd.h>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define ERROR(fmt, ...)                                         \
    do {                                                        \
        fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, \
                ##__VA_ARGS__);                                 \
        exit(1);                                                \
    } while (false)

void child(int read_input, int write_output) {
    while (true) {
        unsigned long n;
        read(read_input, &n, sizeof(n));

        unsigned long result =
            n + 3;  // couldn't be bothered to calculate n'th prime
        write(write_output, &result, sizeof(result));
    }
}

void parent(int write_input, int read_output) {
    while (true) {
        unsigned long n;
        scanf("%lu", &n);

        write(write_input, &n, sizeof(n));

        unsigned long result;
        read(read_output, &result, sizeof(result));

        printf("Result is: %lu\n", result);
    }
}

int main() {
    int input_pipe[2];
    int output_pipe[2];

    if (pipe(input_pipe) < 0) {
        ERROR("Failed to creato anonymous pipe: %s", strerror(errno));
    }
    if (pipe(output_pipe) < 0) {
        ERROR("Failed to creato anonymous pipe: %s", strerror(errno));
    }

    pid_t pid = fork();

    if (pid < 0) {
        ERROR("Failed to fork: %s", strerror(errno));
    }

    if (pid == 0) {
        child(input_pipe[0], output_pipe[1]);
    } else {
        parent(input_pipe[1], output_pipe[0]);
    }
}
