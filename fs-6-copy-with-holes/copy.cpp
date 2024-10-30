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

void copy_data(int src_fd, int dst_fd, size_t count) {
  size_t total_copied = 0;

  while (total_copied < count) {
    size_t to_copy = count > BUF_SIZE ? BUF_SIZE : count;

    ssize_t read_n = read(src_fd, buf, to_copy);
    if (read_n < 0) {
      ERROR("%s", strerror(errno));
    }

    ssize_t wrote_n = write(dst_fd, buf, read_n);
    if (wrote_n < 0) {
      ERROR("%s", strerror(errno));
    }

    if (size_t(read_n) < to_copy) {
      break;
    }

    total_copied += read_n;
  }
}

void copy_with_holes(int src_fd, int dst_fd) {
  off_t end = lseek(src_fd, 0, SEEK_END);
  if (end < 0) {
    ERROR("%s", strerror(errno));
  }

  if (end == 0) {
    return;
  }

  off_t next_data = 0;
  while (true) {
    off_t next_hole = lseek(src_fd, next_data, SEEK_HOLE);
    if (next_hole < 0) {
      ERROR("%s", strerror(errno));
    }

    if (lseek(src_fd, next_data, SEEK_SET) < 0) {
      ERROR("%s", strerror(errno));
    }

    copy_data(src_fd, dst_fd, next_data - next_hole);

    if (next_hole >= end) {
      break;
    }

    next_data = lseek(src_fd, next_hole, SEEK_DATA);
    if (next_data < 0) {
      ERROR("%s", strerror(errno));
    }

    if (lseek(dst_fd, next_data, SEEK_SET) < 0) {
      ERROR("%s", strerror(errno));
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 3) {
    ERROR("Provide source and destionation files");
  }

  int src_fd = open(argv[1], O_RDONLY);
  if (src_fd < 0) {
    ERROR("%s: %s", argv[1], strerror(errno));
  }

  int dst_fd =
      open(argv[2], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (dst_fd < 0) {
    ERROR("%s: %s", argv[2], strerror(errno));
  }
  if (ftruncate(dst_fd, 0) < 0) {
    ERROR("%s", strerror(errno));
  }

  copy_with_holes(src_fd, dst_fd);

  close(src_fd); // ignore errors
  close(dst_fd);
}
