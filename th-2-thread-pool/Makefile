
CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++20 -O0 -g -fsanitize=undefined,address,leak

all: parallel-scheduler.so demo-application

clean:
	rm *.o parallel-scheduler.so demo-application

.PHONY: all clean

demo-application: demo-application.o
	$(CXX) $(CXXFLAGS) -o $@ -L. -l:parallel-scheduler.so $^

parallel-scheduler.so: parallel-scheduler.o
	$(CXX) $(CXXFLAGS) -o $@ -shared $^ 

parallel-scheduler.o: parallel-scheduler.cpp
	$(CXX) $(CXXFLAGS) -c -fPIC parallel-scheduler.cpp

demo-application.o: demo-application.cpp parallel-scheduler.so
	$(CXX) $(CXXFLAGS) -c demo-application.cpp
