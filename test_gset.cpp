#include "gset.h"
#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  GrowOnlySet<int> first, second, third;
  GrowOnlySet<int> joined, op_joined, rejoined;

  first.insert(5);
  first.insert(10);
  first.insert(15);

  bool in = first.in(5) && first.in(10) && first.in(15);
  std::cout << "insert test: " << (in ? "passed" : "failed") << std::endl;

  second.insert(20);
  second.insert(25);

  third.insert(30);
  third.insert(35);

  std::vector<int> expected({5, 10, 15, 20, 25, 30, 35});

  joined.join(first, second, third);
  bool j = std::is_permutation(expected.begin(), expected.end(),
                               joined.elements().begin());
  std::cout << "join test: " << (j ? "passed" : "failed") << std::endl;

  op_joined = first + second + third;
  bool add = std::is_permutation(expected.begin(), expected.end(),
                                 op_joined.elements().begin());
  std::cout << "union test: " << (add ? "passed" : "failed") << std::endl;

  std::vector<GrowOnlySet<int>> joinable_sets({first, second, third});
  joined.join(joinable_sets);
  bool rj = std::is_permutation(expected.begin(), expected.end(),
                                joined.elements().begin());
  std::cout << "rejoin test: " << (rj ? "passed" : "failed") << std::endl;

  auto decompositions = joined.split();
  rejoined.join(decompositions);
  bool eq = (joined == rejoined);
  std::clog << "joined: " << joined << '\n'
            << "rejoined: " << rejoined << std::endl;
  std::cout << "equality test: " << (eq ? "passed" : "failed") << std::endl;

  auto first_and_second = rejoined - third;
  bool s = (first_and_second + third) == rejoined;
  std::clog << "first and second: " << first_and_second << std::endl;
  std::cout << "split test: " << (s ? "passed" : "failed") << std::endl;

  auto none = first_and_second - first - second;
  bool empty = none.elements().empty();
  std::cout << "empty test: " << (empty ? "passed" : "failed") << std::endl;

  return 0;
}
