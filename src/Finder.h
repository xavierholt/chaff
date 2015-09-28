#ifndef CHAFF_FINDER_H
#define CHAFF_FINDER_H

#include "Heap.h"

#include <algorithm>
#include <limits>
#include <queue>
#include <vector>

namespace Chaff
{
  template <class T, class R, class C = std::less<R>, bool Q = true>
  class Finder {
  protected:
    const static C COMPARE;

    struct Ranking {
    protected:
      T mItem;
      R mRank;

    public:
      Ranking() {}
      Ranking(const T& i, const R& r): mItem(i), mRank(r) {}

      const T& item() const {return mItem;}
      const R& rank() const {return mRank;}

      bool operator < (const Ranking& other) const {
        return COMPARE(rank(), other.rank());
      }
    };

    typedef typename std::conditional<Q,
      std::priority_queue<Ranking>,
      Chaff::Heap<Ranking>
    >::type H;

  protected:
    H   mHeap;
    int mMaxCount;
    R   mRank;
    R   mTopRank;
  
  public:
    Finder(int count, const R& rank): mHeap() {
      mMaxCount = count;
      mTopRank  = rank;
      mRank     = rank;
    }

    void clear() {
      mHeap = H();
      mRank = mTopRank;
    }

    void push(const T& item, const R& rank) {
      if(COMPARE(rank, mRank)) {
        mHeap.push(Ranking(item, rank));
        if(mHeap.size() > mMaxCount) {
          mHeap.pop();
          mRank = mHeap.top().rank();
        }
      }
    }

    const R& rank() const {
      return mRank;
    }

    std::vector<T> reap() {
      std::vector<T> results(mHeap.size());
    
      for(int i = mHeap.size() - 1; i >= 0; --i) {
        results[i] = mHeap.top().item();
        mHeap.pop();
      }
    
      clear();
      return results;
    }
  };

  template <class T, class R, bool  Q = true>
  class MaxFinder: public Finder<T, R, std::greater<R>, Q> {
  public:
    static MaxFinder byCount(int count)     {return MaxFinder(count, std::numeric_limits<R>::min());}
    static MaxFinder byScore(const R& rank) {return MaxFinder(std::numeric_limits<int>::max(), rank);}
  public:
    MaxFinder(int count, const R& rank): Finder<T, R, std::greater<R>, Q>(count, rank) {}
  };

  template <class T, class R, bool  Q = true>
  class MinFinder: public Finder<T, R, std::less<R>, Q> {
  public:
    static MinFinder byCount(int count)     {return MinFinder(count, std::numeric_limits<R>::max());}
    static MinFinder byScore(const R& rank) {return MinFinder(std::numeric_limits<int>::max(), rank);}
  public:
    MinFinder(int count, const R& rank): Finder<T, R, std::less<R>, Q>(count, rank) {}
  };
}

#endif
