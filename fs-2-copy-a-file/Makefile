
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O3

all: simple-copy

clean:
	rm *.o simple-copy

.PHONY: all clean

simple-copy: simple-copy.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
