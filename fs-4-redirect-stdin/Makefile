
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O3

all: redirect-cin

clean:
	rm *.o redirect-cin

.PHONY: all clean

redirect-cin: redirect-cin.o
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
