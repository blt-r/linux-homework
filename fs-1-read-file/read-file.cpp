#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

// 128 MiB buffer
constexpr size_t BUF_SIZE = 128 * 1024;
char buf[BUF_SIZE];

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Provide a filename\n";
        exit(1);
    }

    for (int i = 1; i < argc; ++i) {
        int fd = open(argv[i], O_RDONLY);

        if (fd < 0) {
            perror(argv[0]);
            exit(1);
        }

        while (true) {
            ssize_t bytes_read = read(fd, buf, BUF_SIZE);

            if (bytes_read < 0) {
                perror(argv[0]);
                exit(1);
            }

            ssize_t bytes_written = write(STDOUT_FILENO, buf, bytes_read);

            if (bytes_written < 0) {
                perror(argv[0]);
                exit(1);
            }

            if (bytes_read < ssize_t(BUF_SIZE)) {
                break;
            }
        }

        close(fd);  // ignore the error if it happens
    }
}
