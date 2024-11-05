
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O3

all: do-command

clean:
	rm *.o do-command

.PHONY: all clean

do-command: do-command.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
