
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O0 -g -fsanitize=undefined,address,leak

all: first second

clean:
	rm *.o first second

.PHONY: all clean

first: first.o
	$(CXX) $(CXXFLAGS) $^ -o $@

second: second.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
