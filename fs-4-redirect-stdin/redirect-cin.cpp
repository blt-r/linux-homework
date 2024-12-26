#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

void initialize(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Provide input file\n";
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        std::cerr << "Error: " << strerror(errno) << '\n';
        exit(1);
    }

    if (dup2(fd, STDIN_FILENO) < 0) {
        std::cerr << "Error: " << strerror(errno) << '\n';
        exit(1);
    }
}

int main(int argc, char** argv) {
    initialize(argc, argv);

    // read the string
    std::string input;
    std::cin >> input;

    std::string reversed(input.rbegin(), input.rend());

    std::cout << reversed;
    return 0;
}
