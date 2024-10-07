
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O3

all: read-file

clean:
	rm *.o read-file

.PHONY: all clean

read-file: read-file.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
