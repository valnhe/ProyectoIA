CXX	= clang++
CXXFLAGS= -std=c++20 -g -O2 -Wall

all: proyecto

proyecto: instances.o main.o initialsolution.o funcionesAuxiliares.o hc.o
	$(CXX) $(CXXFLAGS) -o $@ $^

instances.o main.o initialsolution.o: instances.h
initialsolution.o: initialsolution.h
funcionesAuxiliares.o: funcionesAuxiliares.h
hc.o: hc.h


clean:
	rm -f *~
	rm -f proyecto \
	      instances.o main.o initialsolution.o hc.o funcionesAuxiliares.o

format:
	clang-format -i *.h *.cc
	      
.PHONY: all clean format