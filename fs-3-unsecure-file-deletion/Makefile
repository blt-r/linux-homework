
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O3

all: hacker-rm

clean:
	rm *.o hacker-rm

.PHONY: all clean

hacker-rm: hacker-rm.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
