#ifndef CHAFF_HEAP_H
#define CHAFF_HEAP_H

#include <stdexcept>
#include <vector>

namespace Chaff
{
  template <class T, class C = std::less<T> >
  class Heap {
  protected:
    std::vector<T> mItems;
    C mCompare;

  public:
    void clear() {
      mItems.clear();
    }

    void pop() {
      int p;
      int c = 0;
      int l = size() - 1;
    
      while(true) {
        p = c;
        c = p * 2 + 1;
        if(c + 1 < l) {
          c += mCompare(mItems[c], mItems[c+1]);
        }
        if(c < l && mCompare(mItems[l], mItems[c])) {
          mItems[p] = mItems[c];
        }
        else {
          break;
        }
      }
    
      mItems[p] = mItems[l];
      mItems.pop_back();
    }

    void push(const T& item) {
      auto c = mItems.size();
      mItems.emplace_back();

      while(c > 0) {
        int p = (c - 1) / 2;
        if(mCompare(item, mItems[p])) break;
        mItems[c] = mItems[p];
        c = p;
      }
    
      mItems[c] = item;
    }

    int size() const {
      return mItems.size();
    }

    const T& top() const {
      return mItems[0];
    }
  };
}

#endif
