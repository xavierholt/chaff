#include "moka/moka.h"

// #define protected public
#include "../Chaff.h"

Moka::Context test("Chaff", [](Moka::Context& it) {
  it.has("Finder", [](Moka::Context& it) {
    // Scope the typedef to avoid accidentally testing the wrong one!
    typedef Chaff::Finder<int,float, std::less<float> > Finder;

    it.has("#constructor()", [](Moka::Context& it) {
      it.should("accept count and score values", []() {
        Finder finder(13, 3.14);
        must_equal(finder.count(),     0U);
        must_equal(finder.score(),  3.14f);
        must_equal(finder.capacity(), 13U);
      });
    });

    it.has("#capacity()", [](Moka::Context& it) {
      it.should("return the maximum number of results", []() {
        Finder finder(13, 3.14);
        must_equal(finder.count(),     0U);
        must_equal(finder.capacity(), 13U);
      });

      it.should("not change after enough results are collected", []() {
        Finder finder(3, 3.14);
        must_equal(finder.count(),    0U);
        must_equal(finder.capacity(), 3U);

        finder.sow(1, 1.1);
        finder.sow(2, 1.2);
        finder.sow(3, 1.3);
        finder.sow(4, 1.4);
        finder.sow(5, 1.5);
        must_equal(finder.count(),    3U);
        must_equal(finder.capacity(), 3U);
      });
    });

    it.has("#clear()", [](Moka::Context& it) {
      it.should("have no visible effect on a new finder", []() {
        Finder finder(13, 3.14);
        must_equal(finder.count(),     0U);
        must_equal(finder.capacity(), 13U);
        must_equal(finder.score(),  3.14f);

        finder.clear();
        must_equal(finder.count(),     0U);
        must_equal(finder.capacity(), 13U);
        must_equal(finder.score(),  3.14f);
      });

      it.should("return the finder to its original state", []() {
        Finder finder(3, 3.14);
        finder.sow(1, 1.1);
        finder.sow(2, 1.2);
        finder.sow(3, 1.3);
        finder.sow(4, 1.4);
        finder.sow(5, 1.5);
        must_equal(finder.count(),    3U);
        must_equal(finder.capacity(), 3U);
        must_equal(finder.score(),  1.3f);

        finder.clear();
        must_equal(finder.count(),    0U);
        must_equal(finder.capacity(), 3U);
        must_equal(finder.score(), 3.14f);
      });
    });

    it.has("#reap()", [](Moka::Context& it) {
      it.should("return an empty vector from a new finder", []() {
        Finder finder(13, 3.14);
        must_equal(finder.reap(), std::vector<int>());
      });

      it.should("return fewer results if not enough were found", []() {
        Finder finder(7, 3.14);
        finder.sow(1, 1.3);
        finder.sow(2, 2.7);
        must_equal(finder.reap(), (std::vector<int>{1, 2}));
      });

      it.should("return at most capacity() results", []() {
        Finder finder(3, 3.14);
        finder.sow(1, 1.3);
        finder.sow(2, 2.7);
        finder.sow(3, 1.7);
        finder.sow(4, 2.0);
        finder.sow(5, 0.3);
        finder.sow(6, 1.9);
        must_equal(finder.reap(), (std::vector<int>{5, 1, 3}));
      });

      it.should("return the results sorted by score", []() {
        Finder finder(5, 3.14);
        finder.sow(1, 1.3);
        finder.sow(2, 2.7);
        finder.sow(3, 7.7); // Too bad to add.
        finder.sow(4, 2.8);
        finder.sow(5, 9.3); // Too bad to add.
        finder.sow(6, 0.9);
        must_equal(finder.reap(), (std::vector<int>{6, 1, 2, 4}));
      });

      it.should("return the finder to its original state", []() {
        Finder finder(1, 3.14);
        must_equal(finder.count(),    0U);
        must_equal(finder.capacity(), 1U);
        must_equal(finder.score(), 3.14f);

        finder.sow(1, 1.3);
        finder.sow(2, 0.7);
        must_equal(finder.count(),    1U);
        must_equal(finder.capacity(), 1U);
        must_equal(finder.score(),  0.7f);

        must_equal(finder.reap(), (std::vector<int>{2}));
        must_equal(finder.count(),    0U);
        must_equal(finder.capacity(), 1U);
        must_equal(finder.score(), 3.14f);
      });
    });

    it.has("#score()", [](Moka::Context& it) {
      it.should("initially be set in the constructor", []() {
        Finder finder(13, 3.14);
        must_equal(finder.score(), 3.14f);
      });

      it.should("update as soon as the finder reaches capacity", []() {
        Finder finder(3, 3.14);
        must_equal(finder.score(), 3.14f);

        finder.sow(1, 0.2);
        must_equal(finder.score(), 3.14f);

        finder.sow(2, 1.2);
        must_equal(finder.score(), 3.14f);

        finder.sow(3, 2.2);
        must_equal(finder.score(),  2.2f);
      });

      it.should("track the worst item in the result set", []() {
        Finder finder(2, 3.14);
        finder.sow(1, 0.2);
        finder.sow(2, 2.2);
        must_equal(finder.score(), 2.2f);

        finder.sow(3, 1.7);
        must_equal(finder.score(), 1.7f);

        finder.sow(4, 0.1);
        must_equal(finder.score(), 0.2f);

        finder.sow(5, 7.9);
        must_equal(finder.score(), 0.2f);

        finder.sow(6, 0.0);
        must_equal(finder.score(), 0.1f);
      });
    });

    it.has("#sow()", [](Moka::Context& it) {
      it.should("accept things better than the current worst", []() {
        Finder finder(13, 3.14);
        must_equal(finder.count(), 0U);

        finder.sow(1, 2.0);
        must_equal(finder.count(), 1U);
      });

      it.should("not accept things worse than the current worst", []() {
        Finder finder(13, 3.14);
        must_equal(finder.count(), 0U);

        finder.sow(1, 4.0);
        must_equal(finder.count(), 0U);
      });

      it.should("not accept things equal to the current worst", []() {
        Finder finder(13, 3.14);
        must_equal(finder.count(), 0U);

        finder.sow(1, 3.14);
        must_equal(finder.count(), 0U);
      });

      it.should("stay within the maximum capacity", []() {
        Finder finder(1, 3.14);
        must_equal(finder.count(), 0U);

        finder.sow(1, 3.0);
        must_equal(finder.count(), 1U);

        finder.sow(2, 2.0);
        must_equal(finder.count(), 1U);
      });

      it.should("update the current worst score", []() {
        Finder finder(2, 3.14);
        must_equal(finder.score(), 3.14f);

        finder.sow(1, 3.0);
        must_equal(finder.score(), 3.14f);

        finder.sow(2, 2.0);
        must_equal(finder.score(),  3.0f);

        finder.sow(3, 1.0);
        must_equal(finder.score(),  2.0f);
      });
    });

    it.has("#thing()", [](Moka::Context& it) {
      it.should("return the only thing in the finder", []() {
        Finder finder(13, 3.14);
        finder.sow(1, 1.7);
        must_equal(finder.thing(), 1);
      });

      it.should("throw an underflow error if the finder is empty", []() {
        Finder finder(13, 3.14);
        must_throw(std::out_of_range, [&]() {
          std::cout << finder.thing();
        });
      });

      it.should("return the worst thing in the finder", []() {
        Finder finder(13, 3.14);
        finder.sow(1, 0.7);
        finder.sow(2, 8.6);
        finder.sow(3, 2.7);
        finder.sow(4, 0.1);
        finder.sow(5, 1.7);
        must_equal(finder.thing(), 3);
      });

      it.should("track the worst thing in the finder", []() {
        Finder finder(3, 3.14);
        finder.sow(1, 0.7);
        must_equal(finder.thing(), 1);

        finder.sow(2, 8.6); // Too bad to add.
        must_equal(finder.thing(), 1);

        finder.sow(3, 2.7);
        must_equal(finder.thing(), 3);

        finder.sow(4, 0.1); // Max score is now 2.7
        must_equal(finder.thing(), 3);

        finder.sow(5, 3.1); // Too bad to add.
        must_equal(finder.thing(), 3);

        finder.sow(6, 1.9); // Kicks out 3 = 2.7
        must_equal(finder.thing(), 6);

        finder.sow(7, 0.3); // Kicks out 6 = 1.9
        must_equal(finder.thing(), 1);

        finder.sow(8, 0.1); // Kicks out 1 = 0.7
        must_equal(finder.thing(), 7);
      });
    });
  });

  it.has("MinFinder", [](Moka::Context& it) {
    // Scope the typedef to avoid accidentally testing the wrong one!
    typedef Chaff::MinFinder<int,float> Finder;

    it.has("#constructor()", [](Moka::Context& it) {
      it.should("create an empty finder by default", []() {
        Finder finder;
        must_equal(finder.count(), 0U);
        must_equal(finder.score(), std::numeric_limits<float>::max());
      });

      it.should("accept count and score values", []() {
        Finder finder(3, 4.2);
        must_equal(finder.count(),    0U);
        must_equal(finder.score(),  4.2f);
        must_equal(finder.capacity(), 3U);
      });
    });

    it.has("#byCount()", [](Moka::Context& it) {
      it.should("create an empty finder for the given count", []() {
        Finder finder = Finder::byCount(7);
        must_equal(finder.count(), 0U);
        must_equal(finder.score(), finder.defaultScore());
        must_equal(finder.capacity(), 7U);
      });
    });

    it.has("#byScore()", [](Moka::Context& it) {
      it.should("create an empty finder for the given score", []() {
        Finder finder = Finder::byScore(4.9);
        must_equal(finder.count(),   0U);
        must_equal(finder.score(), 4.9f);
        must_equal(finder.capacity(), finder.defaultCount());
      });
    });

    it.has("#defaultCount()", [](Moka::Context& it) {
      it.should("return pretty much infinity", []() {
        must_equal(Finder::defaultCount(), std::numeric_limits<Finder::Size>::max());
      });
    });

    it.has("#defaultScore()", [](Moka::Context& it) {
      it.should("return pretty much infinity", []() {
        must_equal(Finder::defaultScore(), std::numeric_limits<float>::max());
      });
    });
  });

  it.has("MaxFinder", [](Moka::Context& it) {
    // Scope the typedef to avoid accidentally testing the wrong one!
    typedef Chaff::MaxFinder<int,float> Finder;

    it.has("#constructor()", [](Moka::Context& it) {
      it.should("create an empty finder by default", []() {
        Finder finder;
        must_equal(finder.count(), 0U);
        must_equal(finder.score(), std::numeric_limits<float>::min());
      });

      it.should("accept count and score values", []() {
        Finder finder(3, 4.2);
        must_equal(finder.count(),    0U);
        must_equal(finder.score(),  4.2f);
        must_equal(finder.capacity(), 3U);
      });
    });

    it.has("#byCount()", [](Moka::Context& it) {
      it.should("create an empty finder for the given count", []() {
        Finder finder = Finder::byCount(7);
        must_equal(finder.count(), 0U);
        must_equal(finder.score(), finder.defaultScore());
        must_equal(finder.capacity(), 7U);
      });
    });

    it.has("#byScore()", [](Moka::Context& it) {
      it.should("create an empty finder for the given score", []() {
        Finder finder = Finder::byScore(4.9);
        must_equal(finder.count(),   0U);
        must_equal(finder.score(), 4.9f);
        must_equal(finder.capacity(), finder.defaultCount());
      });
    });

    it.has("#defaultCount()", [](Moka::Context& it) {
      it.should("return pretty much infinity", []() {
        must_equal(Finder::defaultCount(), std::numeric_limits<Finder::Size>::max());
      });
    });

    it.has("#defaultScore()", [](Moka::Context& it) {
      it.should("return pretty much negative infinity", []() {
        must_equal(Finder::defaultScore(), std::numeric_limits<float>::min());
      });
    });
  });
});

int main() {
  return test.run();
}
