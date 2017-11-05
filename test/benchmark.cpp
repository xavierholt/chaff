#include "../Chaff.h"

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

typedef Chaff::MinFinder<int,int> MinFin;
typedef Chaff::MaxFinder<int,int> MaxFin;

int main(int argc, char* argv[]) {
  if(argc < 4 || argc > 5) {
    printf("USAGE: %s data-count find-count test-count [seed]\n", argv[0]);
    exit(1);
  }

  const int DATA = std::atoi(argv[1]);
  const int FIND = std::atoi(argv[2]);
  const int REPS = std::atoi(argv[3]);
  const int SEED = (argc == 5)? std::atoi(argv[4]): time(0);
  printf("%s %i %i %i %i\n", argv[0], DATA, FIND, REPS, SEED);

  srand(SEED);
  std::vector<int> data(DATA);
  for(int i = 0; i < DATA ; ++i) {
    data[i] = i;
  }

  NS  cmin_time  = 0;
  NS  cmax_time  = 0;
  NS  sort_time  = 0;
  NS  temp_time;

  int cmin_found = 0;
  int cmax_found = 0;

  for(int i = 0; i < REPS; ++i) {
    std::random_shuffle(data.begin(), data.end());

    temp_time = nanoseconds();
    /*** MinFinder ************************/
    MinFin fmin = MinFin::byCount(FIND);
    for(int i = 0; i < DATA; ++i) fmin.sow(data[i], data[i]);
    std::vector<int> cmin_data(fmin.reap());
    /*** End MinFinder ********************/
    cmin_time += nanoseconds() - temp_time;

    temp_time = nanoseconds();
    /*** MaxFinder ************************/
    MaxFin fmax = MaxFin::byCount(FIND);
    for(int i = 0; i < DATA; ++i) fmax.sow(data[i], data[i]);
    std::vector<int> cmax_data(fmax.reap());
    /*** End MaxFinder ********************/
    cmax_time += nanoseconds() - temp_time;

    temp_time = nanoseconds();
    /*** Sort ALL the Things **************/
    int n = std::min(DATA, FIND);
    std::sort(data.begin(), data.end());
    std::vector<int> smin_data(data.begin(),  data.begin()  + n);
    std::vector<int> smax_data(data.rbegin(), data.rbegin() + n);
    /*** End Sorting **********************/
    sort_time += nanoseconds() - temp_time;

    cmin_found += (cmin_data == smin_data);
    cmax_found += (cmax_data == smax_data);
  }

  printf("MinFinder %10.6fs (%i / %i)\n", double(cmin_time) / NANO, cmin_found, REPS);
  printf("MaxFinder %10.6fs (%i / %i)\n", double(cmax_time) / NANO, cmax_found, REPS);
  printf("Full Sort %10.6fs (%i / %i)\n", double(sort_time) / NANO, REPS,       REPS);

  if(cmin_found != REPS || cmax_found != REPS) {
    printf("Result mismatch!\n\n");
    exit(2);
  }

  printf("\n");
  return 0;
}
