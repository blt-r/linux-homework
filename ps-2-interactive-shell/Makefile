
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O0 -g -fsanitize=undefined,address,leak

all: interactive-shell

clean:
	rm *.o interactive-shell

.PHONY: all clean

interactive-shell: interactive-shell.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
