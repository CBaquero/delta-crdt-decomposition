#include "2pset.h"
#include <algorithm>
#include <iostream>
#include <vector>

int main(void) {
  delta::TwoPhaseSet<int> first, second, third;
  delta::TwoPhaseSet<int> joined, op_joined, rejoined;

  first.insert(5);
  first.insert(10);
  first.insert(15);

  bool insert = first.contains(5) && first.contains(10) && first.contains(15);
  std::cout << "insert test: " << (insert ? "passed" : "failed") << std::endl;

  first.insert(10);
  first.remove(15);

  bool remove = first.contains(5) && first.contains(10) && !first.contains(15);
  std::cout << "remove test: " << (remove ? "passed" : "failed") << std::endl;

  second.insert(15); // cannot be re-added when joinning
  second.insert(20);

  third.insert(25);
  third.insert(30);

  std::vector<int> expected({5, 10, 20, 25, 30});

  joined.join(first, second, third);
  bool join = std::is_permutation(expected.begin(), expected.end(),
                                  joined.elements().begin());
  std::cout << "join test: " << (join ? "passed" : "failed") << std::endl;

  op_joined = first + second + third;
  bool add = std::is_permutation(expected.begin(), expected.end(),
                                 op_joined.elements().begin());
  std::cout << "union test: " << (add ? "passed" : "failed") << std::endl;

  joined.join({first, second, third});
  bool rejoin = std::is_permutation(expected.begin(), expected.end(),
                                    joined.elements().begin());
  std::cout << "rejoin test: " << (rejoin ? "passed" : "failed") << std::endl;

  auto joinable_sets2 = joined.split();
  rejoined.join(joinable_sets2);
  bool eq = (joined == rejoined);
  std::clog << "joined: " << joined << '\n'
            << "rejoined: " << rejoined << std::endl;
  std::cout << "equality test: " << (eq ? "passed" : "failed") << std::endl;

  auto first_and_second = rejoined - third;
  bool split = (first_and_second + third) == rejoined;
  std::clog << "first and second: " << first_and_second << std::endl;
  std::cout << "split test: " << (split ? "passed" : "failed") << std::endl;

  auto none = first_and_second - first - second;
  bool empty = none.elements().empty();
  std::cout << "empty test: " << (empty ? "passed" : "failed") << std::endl;

  int errors =
      !insert + !remove + !join + !add + !rejoin + !eq + !split + !empty;
  return errors;
}
