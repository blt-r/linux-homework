
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O3

all: copy

clean:
	rm *.o copy

.PHONY: all clean

copy: copy.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
