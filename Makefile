FLAGS = -O3 -Wall -Wextra -Werror

all: example.out benchmark.out unit-tests.out
benchmark.out: test/benchmark.cpp Chaff.h
	${CXX} $(FLAGS) -o $@ $< $(LIBS)
example.out: test/example.cpp Chaff.h
	${CXX} $(FLAGS) -o $@ $< $(LIBS)
unit-tests.out: test/unit-tests.cpp Chaff.h
	${CXX} $(FLAGS) -std=c++11 -o $@ $< $(LIBS)

test: all
	./example.out
	./unit-tests.out
	./benchmark.out 1000000 1 10
	./benchmark.out 1000000 10 10
	./benchmark.out 1000000 100 10
	./benchmark.out 1000000 1000 10
	./benchmark.out 1000000 10000 10
	./benchmark.out 1000000 100000 10

subtree-init:
	git subtree add  --prefix test/moka https://github.com/xavierholt/moka.git master --squash
subtree-pull:
	git subtree pull --prefix test/moka https://github.com/xavierholt/moka.git master --squash

clean:
	rm -f example.out benchmark.out unit-tests.out
