FLAGS = -std=c++11 -O3 -Wall -Werror

all: example.out benchmark.out
example.out: tst/example.cpp src/Finder.h
	${CXX} $(FLAGS) -o $@ $<
benchmark.out: tst/benchmark.cpp src/Finder.h
	${CXX} $(FLAGS) -o $@ $<
test: all
	./example.out
	./benchmark.out 1000000      1 10
	./benchmark.out 1000000     10 10
	./benchmark.out 1000000    100 10
	./benchmark.out 1000000   1000 10
	./benchmark.out 1000000  10000 10
	./benchmark.out 1000000 100000 10
clean:
	rm example.out benchmark.out
