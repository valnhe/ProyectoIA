CXX	= clang++
CXXFLAGS= -std=c++20 -g -O2 -Wall

all: proyecto

proyecto: instances.o main.o initialsolution.o
	$(CXX) $(CXXFLAGS) -o $@ $^

instances.o main.o initialsolution.o: instances.h
initialsolution.o: initialsolution.h

clean:
	rm -f *~
	rm -f proyecto \
	      instances.o main.o initialsolution.o

format:
	clang-format -i *.h *.cc
	      
.PHONY: all clean format