#include <sys/wait.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#define ERROR(fmt, ...)                                         \
    do {                                                        \
        fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, \
                ##__VA_ARGS__);                                 \
        exit(1);                                                \
    } while (false)

int do_command(const char* name, char* const args[]) {
    int pid = fork();
    if (pid < 0) {
        ERROR("%s", strerror(errno));
    }

    if (pid == 0) {
        execvp(name, args);
        ERROR("%s", strerror(errno));
    }

    int status;

    if (waitpid(pid, &status, 0) < 0) {
        ERROR("%s", strerror(errno));
    }

    return WEXITSTATUS(status);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        ERROR("Provide program name");
    }

    long t0 = time(nullptr);
    int code = do_command(argv[1], argv + 1);
    long duration = time(nullptr) - t0;

    printf("\nCommand completed with %d exit code and took %ld seconds\n", code,
           duration);
}
