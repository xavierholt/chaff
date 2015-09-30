FLAGS = --std=c++11 -O3 -Wall -Werror

all: example.out benchmark.out
example.out: tst/example.cpp src/Finder.h
	${CXX} $(FLAGS) -o $@ $<
benchmark.out: tst/benchmark.cpp src/Finder.h
	${CXX} $(FLAGS) -o $@ $<
test: all
	./example.out
	./benchmark.out 1000000      1 50
	./benchmark.out 1000000     10 50
	./benchmark.out 1000000    100 50
	./benchmark.out 1000000   1000 50
	./benchmark.out 1000000  10000 50
	./benchmark.out 1000000 100000 50
clean:
	rm example.out benchmark.out
