CXX	= clang++
CXXFLAGS= -std=c++20 -g -O2 -Wall

all: proyecto

proyecto: readInstance.o main.o randomSolution.o funcionesAuxiliares.o hc.o
	$(CXX) $(CXXFLAGS) -o $@ $^

readInstance.o main.o randomSolution.o: readInstance.h
randomSolution.o: randomSolution.h
funcionesAuxiliares.o: funcionesAuxiliares.h
hc.o: hc.h


clean:
	rm -f *~
	rm -f proyecto \
	      readInstance.o main.o randomSolution.o hc.o funcionesAuxiliares.o

format:
	clang-format -i *.h *.cc
	      
.PHONY: all clean format