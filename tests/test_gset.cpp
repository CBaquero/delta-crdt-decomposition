#include "gset.h"
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  // NOLINTBEGIN (*-readability)
  delta::GrowOnlySet<int> first, second, third;
  delta::GrowOnlySet<int> joined, op_joined, rejoined;

  first.insert(5);
  first.insert(10);
  first.insert(15);

  bool insert = first.contains(5) && first.contains(10) && first.contains(15);
  std::cout << "insert test: " << (insert ? "passed" : "failed") << std::endl;

  second.insert(20);
  second.insert(25);

  third.insert(30);
  third.insert(35);

  std::vector<int> expected({5, 10, 15, 20, 25, 30, 35});

  joined.join(first, second, third);
  bool join = std::is_permutation(expected.begin(), expected.end(),
                                  joined.elements().begin());
  std::cout << "join test: " << (join ? "passed" : "failed") << std::endl;

  op_joined = first + second + third;
  bool add = std::is_permutation(expected.begin(), expected.end(),
                                 op_joined.elements().begin());
  std::cout << "add test: " << (add ? "passed" : "failed") << std::endl;

  joined.join({first, second, third});
  bool rejoin = std::is_permutation(expected.begin(), expected.end(),
                                    joined.elements().begin());
  std::cout << "rejoin test: " << (rejoin ? "passed" : "failed") << std::endl;

  auto decompositions = joined.split();
  rejoined.join(decompositions);
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

  int errors = !insert + !join + !add + !rejoin + !eq + !split + !empty;
  return errors;
  // NOLINTEND (*-readability)
}
