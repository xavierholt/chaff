#ifndef CHAFF_H
#define CHAFF_H

#include <limits>
#include <queue>
#include <stdexcept>

namespace Chaff
{
  template <class T, class S, class C>
  class Finder {
  protected:
    struct Ranking {
    protected:
      T mThing;
      S mScore;
      C mCompare;

    public:
      Ranking(const T& t, const S& s): mThing(t), mScore(s) {}

      const T& thing() const {return mThing;}
      const S& score() const {return mScore;}

      bool operator < (const Ranking& other) const {
        return mCompare(score(), other.score());
      }
    };

  public:
    typedef typename std::priority_queue<Ranking> Heap;
    typedef typename Heap::size_type Size;
    typedef T Thing;
    typedef S Score;

  protected:
    Heap mHeap;
    Size mCapacity;
    S    mTopScore;
    S    mMaxScore;
    C    mCompare;

  public:
    Finder(Size count, const S& score): mHeap() {
      mCapacity = count;
      mTopScore = score;
      mMaxScore = score;
    }

    Size capacity() const {
      return mCapacity;
    }

    void clear() {
      mHeap     = Heap();
      mTopScore = mMaxScore;
    }

    Size count() const {
      return mHeap.size();
    }

    std::vector<T> reap() {
      std::vector<T> results(mHeap.size());

      Size i = mHeap.size();
      while(i > 0) {
        results[--i] = mHeap.top().thing();
        mHeap.pop();
      }

      clear();
      return results;
    }

    const S& score() const {
      return mTopScore;
    }

    void sow(const T& thing, const S& score) {
      if(mCompare(score, mTopScore)) {
        mHeap.push(Ranking(thing, score));
        Size size = mHeap.size();

        if(size > mCapacity) {
          mHeap.pop();
        }

        if(size >= mCapacity) {
          mTopScore = mHeap.top().score();
        }
      }
    }

    const T& thing() const {
      if(mHeap.size() < 1) {
        throw std::out_of_range("Tried to access an empty Chaff::Finder.");
      }
      else {
        return mHeap.top().thing();
      }
    }
  };

  template <class T, class S>
  class MaxFinder: public Finder<T, S, std::greater<S> > {
  public:
    typedef typename Finder<T, S, std::greater<S> >::Size Size;
    typedef T Thing;
    typedef S Score;

    static Size defaultCount() {return std::numeric_limits<Size>::max();}
    static S    defaultScore() {return std::numeric_limits<S>::min();}

    static MaxFinder byCount(Size count)     {return MaxFinder(count, defaultScore());}
    static MaxFinder byScore(const S& score) {return MaxFinder(defaultCount(), score);}
  public:
    MaxFinder(): Finder<T, S, std::greater<S> >(defaultCount(), defaultScore()) {}
    MaxFinder(Size count, const S& score): Finder<T, S, std::greater<S> >(count, score) {}
  };

  template <class T, class S>
  class MinFinder: public Finder<T, S, std::less<S> > {
  public:
    typedef typename Finder<T, S, std::greater<S> >::Size Size;
    typedef T Thing;
    typedef S Score;

    static Size defaultCount() {return std::numeric_limits<Size>::max();}
    static S    defaultScore() {return std::numeric_limits<S>::max();}

    static MinFinder byCount(Size count)     {return MinFinder(count, defaultScore());}
    static MinFinder byScore(const S& score) {return MinFinder(defaultCount(), score);}
  public:
    MinFinder(): Finder<T, S, std::less<S> >(defaultCount(), defaultScore()) {}
    MinFinder(Size count, const S& score): Finder<T, S, std::less<S> >(count, score) {}
  };
}

#endif
