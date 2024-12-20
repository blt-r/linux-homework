
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O0 -g -fsanitize=undefined,address,leak

all: signal-echo

clean:
	rm *.o signal-echo

.PHONY: all clean

signal-echo: signal-echo.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
