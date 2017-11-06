FLAGS = -O3 -Wall -Wextra -Werror

all: example.out benchmark.out unit-tests.out
%.out: test/%.cpp Chaff.h
	${CXX} $(FLAGS) -o $@ $< $(LIBS)

test: all
	./example.out
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
	rm -f example.out benchmark.out
