
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O3

all: manual-append

clean:
	rm *.o manual-append

.PHONY: all clean

manual-append: manual-append.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
