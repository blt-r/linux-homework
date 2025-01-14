#pragma once

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string_view>

#define ERROR(fmt, ...)                                         \
    do {                                                        \
        fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, \
                ##__VA_ARGS__);                                 \
        exit(1);                                                \
    } while (false)

struct SharedArray {
    int fd;
    void* ptr;
    size_t len;

    SharedArray(std::string_view name, size_t len) : len(len) {
        std::stringstream path;
        path << name << '-' << len;
        auto path_str = std::move(path).str();

        fd = shm_open(path_str.c_str(), O_RDWR | O_CREAT, 0644);

        if (fd < 0) {
            ERROR("Failed to shm_open: %s", strerror(errno));
        }

        if (ftruncate(fd, len) < 0) {
            ERROR("Failed to truncate: %s", strerror(errno));
        }

        ptr = mmap(nullptr, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

        if (ptr == nullptr) {
            ERROR("Failed to mmap: %s", strerror(errno));
        }
    }

    ~SharedArray() {
        if (munmap(ptr, len) < 0) {
            ERROR("Failed to munmap: %s", strerror(errno));
        }

        if (close(fd) < 0) {
            ERROR("Failed to close: %s", strerror(errno));
        }
    }
};
