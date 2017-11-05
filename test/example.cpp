#include "../Chaff.h"

#include <cstdlib>
#include <iostream>
#include <vector>

typedef Chaff::MaxFinder<int,int> MyFinder;
const int N = 1000000;

int main() {
  // ONE MILLION random integers!
  std::vector<int> things(N);
  for(int i = 0; i < N; ++i) {
    things[i] = rand();
  }

  // Inspect them all to find the best three...
  MyFinder finder = MyFinder::byCount(3);
  for(int i = 0; i < N; ++i) {
    finder.sow(things[i], things[i]);
  }

  // And show us what you found!
  std::vector<int> found(finder.reap());
  std::cout << "The best three things are:\n";
  for(int i = 0; i < 3; ++i) {
    std::cout << " - " << found[i] << '\n';
  }

  std::cout << "Aren't they amazing?\n\n";
  return 0;
}
