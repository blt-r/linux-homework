
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O0 -g -fsanitize=undefined,address,leak

all: array-sum

clean:
	rm *.o array-sum

.PHONY: all clean

array-sum: array-sum.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
