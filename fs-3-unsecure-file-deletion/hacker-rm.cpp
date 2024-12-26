#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>

#define ERROR(fmt, ...)                                         \
    do {                                                        \
        fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, \
                ##__VA_ARGS__);                                 \
        exit(1);                                                \
    } while (false)

// 128 MiB buffer
constexpr size_t BUF_SIZE = 128 * 1024;
char buf[BUF_SIZE] = {};

void hacker_delete(const char* filename) {
    int fd = open(filename, O_WRONLY);
    if (fd < 0) {
        ERROR("%s", strerror(errno));
    }

    ssize_t write_remaining = lseek(fd, 0, SEEK_END);
    if (write_remaining < 0) {
        ERROR("%s", strerror(errno));
    }
    if (lseek(fd, 0, SEEK_SET) < 0) {
        ERROR("%s", strerror(errno));
    }

    while (write_remaining > 0) {
        ssize_t to_write = std::min(ssize_t(BUF_SIZE), write_remaining);

        ssize_t bytes_written = write(fd, buf, to_write);
        if (bytes_written < 0) {
            ERROR("%s", strerror(errno));
        }

        write_remaining -= bytes_written;
    }

    // ignore the error if it happens
    close(fd);

    if (unlink(filename) < 0) {
        ERROR("%s", strerror(errno));
    }
}

int main(int argc, char* argv[]) {
    if (argc <= 2) {
        ERROR("Provide files to delete");
    }

    for (int i = 1; i < argc; ++i) {
        hacker_delete(argv[i]);
    }
}
