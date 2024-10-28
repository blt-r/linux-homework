#include <iostream>

#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// 128 MiB buffer
constexpr size_t BUF_SIZE = 128 * 1024;
char buf[BUF_SIZE];

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Provide source and destination filenames\n";
    exit(1);
  }

  struct stat src_stat = {};
  if (stat(argv[1], &src_stat) < 0) {
    perror(argv[0]);
    exit(1);
  }

  int src_fd = open(argv[1], O_RDONLY);
  if (src_fd < 0) {
    perror(argv[0]);
    exit(1);
  }
  int dst_fd = open(argv[2], O_WRONLY | O_CREAT, src_stat.st_mode);
  if (dst_fd < 0) {
    perror(argv[0]);
    exit(1);
  }

  while (true) {
    ssize_t bytes_read = read(src_fd, buf, BUF_SIZE);

    if (bytes_read < 0) {
      perror(argv[0]);
      exit(1);
    }

    ssize_t bytes_written = write(dst_fd, buf, bytes_read);

    if (bytes_written < 0) {
      perror(argv[0]);
      exit(1);
    }

    if (bytes_read < ssize_t(BUF_SIZE)) {
      break;
    }
  }

  off_t len = lseek(dst_fd, 0, SEEK_CUR);
  if (len < 0) {
    perror(argv[0]);
    exit(1);
  }

  if (ftruncate(dst_fd, len) < 0) {
    perror(argv[0]);
    exit(1);
  }

  // ignore the error if it happens
  close(src_fd);
  close(dst_fd);
}
