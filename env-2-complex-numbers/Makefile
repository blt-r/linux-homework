
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O3

all: main

clean:
	rm *.o main

.PHONY: all clean

main: main.o Complex.o SortComplex.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
