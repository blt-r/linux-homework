#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <fcntl.h>
#include <unistd.h>

#define ERROR(fmt, ...)                                                        \
  do {                                                                         \
    fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);    \
    exit(1);                                                                   \
  } while (false)

// 128 MiB buffer
constexpr size_t BUF_SIZE = 128 * 1024;
char buf[BUF_SIZE] = {};

// Cannot open file with O_APPEND; cannot use lseek.
void manual_append(int fd, char* msg,size_t msg_len) {
  while (true) {
    int read_n = read(fd, buf, BUF_SIZE);

    if (read_n < 0) {
      ERROR("%s", strerror(errno));
    }

    if (size_t(read_n) < BUF_SIZE) {
      break;
    }
  }

  if (write(fd, msg, msg_len) < 0) {
    ERROR("%s", strerror(errno));
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    ERROR("Provide file to append to");
  }

  int fd_1 = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd_1 < 0) {
    ERROR("%s: %s", argv[1], strerror(errno));
  }
  int fd_2 = dup(fd_1);
  if (fd_2 < 0) {
    ERROR("dup failed: %s", strerror(errno));
  }

  char first_line[] = "first line\n";
  char second_line[] = "second line\n";
  manual_append(fd_1, first_line, sizeof(first_line) - 1);
  manual_append(fd_2, second_line, sizeof(second_line) - 1);

  close(fd_1); // ignore errors
  close(fd_2);
}
