#include "../src/Finder.h"

#include <algorithm>
#include <cstdio>
#include <ctime>

typedef long long int NS;
const NS NANO = 1000000000LL;

NS nanoseconds() {
  timespec time;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time);
  return NANO * time.tv_sec + time.tv_nsec;
}

NS bench(const std::function<void()>& callback) {
  NS start = nanoseconds();
  callback();
  return nanoseconds() - start;
}

void print(char id, const std::vector<int>& data) {
  printf("%c: ", id);
  for(int i: data) printf("%i ", i);
  printf("\n");
}

int main() {
  srand(time(NULL));

  const int REPS = 100;
  const int DATA = 1000000;
  const int FIND = 2000;

  int* data = new int[DATA];
  for(int i = 0; i < DATA ; ++i) {
    data[i] = i;
  }

  NS qt = 0;
  NS ht = 0;
  NS st = 0;

  int qf = 0;
  int hf = 0;
  int sf = 0;

  for(int i = 0; i < REPS; ++i) {
    std::random_shuffle(data, data + DATA);

    std::vector<int> qd;
    std::vector<int> hd;
    std::vector<int> sd;
    
    qt += bench([&]() {
      auto q = Chaff::MinFinder<int, int, true>::byCount(FIND);
      for(int i = 0; i < DATA; ++i) q.push(data[i], data[i]);
      qd = q.reap();
    });

    ht += bench([&]() {
      auto h = Chaff::MinFinder<int, int, false>::byCount(FIND);
      for(int i = 0; i < DATA; ++i) h.push(data[i], data[i]);
      hd = h.reap();
    });

    st += bench([&]() {
      std::sort(data, data + DATA);
      sd = std::vector<int>(data, data + FIND);
    });

    // print('q', qd);
    // print('h', hd);
    // print('s', sd);

    qf += (qd == sd);
    hf += (hd == sd);
    sf += 1;
  }

  printf("std::priority_queue %10.6f (%i / %i)\n", double(qt) / NANO, qf, sf);
  printf("Chaff::Heap         %10.6f (%i / %i)\n", double(ht) / NANO, hf, sf);
  printf("std::sort           %10.6f (%i / %i)\n", double(st) / NANO, sf, sf);

  return 0;
}
