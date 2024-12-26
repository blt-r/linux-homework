#include <pthread.h>
#include <cerrno>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <span>
#include <vector>

#define ERROR(fmt, ...)                                         \
    do {                                                        \
        fprintf(stderr, "%s:%d: " fmt "\n", __FILE__, __LINE__, \
                ##__VA_ARGS__);                                 \
        exit(1);                                                \
    } while (false)

unsigned long parse_number(char* str) {
    char* end = nullptr;
    unsigned long n = strtoul(str, &end, 10);
    if (*end != '\0') {
        ERROR("Invalid number");
    }
    return n;
}

struct ThreadArgs {
    int* start;
    int* end;
    int result;
};

void* sum_thread(void* args_untyped) {
    ThreadArgs* args = (ThreadArgs*)args_untyped;

    int result = 0;

    for (int* ptr = args->start; ptr != args->end; ++ptr) {
        result += *ptr;
    }

    args->result = result;

    return nullptr;
}

int sum_array_with_threads(std::span<int> array, size_t thread_count) {
    std::vector<pthread_t> threads(thread_count);
    std::vector<ThreadArgs> args(thread_count);

    size_t butch_size = array.size() / thread_count;

    for (size_t i = 0; i < thread_count; ++i) {
        size_t end_idx =
            (i < thread_count - 1) ? (i + 1) * butch_size : array.size();

        args[i] = ThreadArgs{.start = array.data() + i * butch_size,
                             .end = array.data() + end_idx,
                             .result = 0};

        pthread_create(&threads[i], nullptr, sum_thread, &args[i]);
    }

    int total = 0;

    for (size_t i = 0; i < thread_count; ++i) {
        pthread_join(threads[i], nullptr);
        total += args[i].result;
    }

    return total;
}

int sum_array(std::span<int> array) {
    int total = 0;
    for (int num : array) {
        total += num;
    }

    return total;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        ERROR("Usage: %s ARRAY_SIZE THREAD_COUNT", argv[0]);
    }

    unsigned long array_size = parse_number(argv[1]);
    unsigned long thread_count = parse_number(argv[2]);

    std::vector<int> array(array_size);

    {
        auto t0 = std::chrono::high_resolution_clock::now();

        for (int& num : array) {
            // num = 7;
            num = rand() % 16;
        }

        auto t1 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double>(t1 - t0);
        std::cout << "Time spent generating numbers: " << duration << '\n';
    }

    {
        auto t0 = std::chrono::high_resolution_clock::now();

        int total = sum_array(array);

        auto t1 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double>(t1 - t0);
        std::cout << "Time spent without threads: " << duration << '\n';
        std::cout << "Total: " << total << '\n';
    }

    {
        auto t0 = std::chrono::high_resolution_clock::now();

        int total = sum_array_with_threads(array, thread_count);

        auto t1 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration<double>(t1 - t0);
        std::cout << "Time spent with " << thread_count
                  << " threads: " << duration << '\n';
        std::cout << "Total: " << total << '\n';
    }
}
