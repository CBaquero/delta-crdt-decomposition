#include "lwwset.h"
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
  LastWriterWinsSet<int> first, second, third;
  LastWriterWinsSet<int> joined, op_joined, rejoined;

  first.insert(5, 1);
  first.insert(10, 2);

  bool in = first.in(5) && first.in(10);
  std::cout << "insert test: " << (in ? "passed" : "failed") << std::endl;

  try {
    std::clog << "before insertion: " << first << std::endl;
    first.insert(10, 1);
  } catch (std::invalid_argument &e) {
    std::clog << "after insertion: " << first << std::endl;
    std::cout << "monotonicity test: passed" << std::endl;
  }

  first.insert(15, 2);
  first.remove(15, 3);

  second.insert(15, 3); // add-wins
  second.insert(20, 4);

  third.insert(25, 1);
  third.insert(30, 1);

  const std::vector<int> expected({5, 10, 15, 20, 25, 30});

  joined.join(first, second, third);
  bool j = std::is_permutation(expected.begin(), expected.end(),
                               joined.elements().begin());
  std::cout << "remove test: " << (j ? "passed" : "failed") << std::endl;
  std::cout << "join test: " << (j ? "passed" : "failed") << std::endl;

  op_joined = first + second + third;
  bool add = std::is_permutation(expected.begin(), expected.end(),
                                 op_joined.elements().begin());
  std::cout << "union test: " << (add ? "passed" : "failed") << std::endl;

  std::vector<LastWriterWinsSet<int>> joinable_sets({first, second, third});
  joined.join(joinable_sets);
  bool rj = std::is_permutation(expected.begin(), expected.end(),
                                joined.elements().begin());
  std::cout << "rejoin test: " << (rj ? "passed" : "failed") << std::endl;

  auto joinable_sets2 = joined.split();
  rejoined.join(joinable_sets2);

  std::clog << "joined: " << joined << '\n'
            << "rejoined: " << rejoined << std::endl;
  std::cout << "equality test: " << (joined == rejoined ? "passed" : "failed")
            << std::endl;

  auto first_and_second = rejoined - third;
  bool s = (first_and_second + third) == rejoined;
  std::clog << "first and second: " << first_and_second << std::endl;
  std::cout << "split test: " << (s ? "passed" : "failed") << std::endl;

  auto none = first_and_second - first - second;
  bool empty = none.elements().empty();
  std::cout << "empty test: " << (empty ? "passed" : "failed") << std::endl;

  LastWriterWinsSet<int> ours, theirs;

  ours.insert(1, 1);
  ours.remove(1, 2);  // remote addition wins - not present
  ours.insert(10, 7); // local addition wins - present
  ours.insert(2, 3);  // remote is ahead - not present
  ours.insert(3, 4);  // synchronized - not present
  ours.insert(4, 5);  // local only - present

  theirs.insert(1, 2); // remote addition wins - not present
  theirs.insert(10, 6);
  theirs.remove(10, 7); // local addition wins - present
  theirs.insert(2, 4);  // remote is ahead - not present
  theirs.insert(3, 4);  // synchronized - not present

  auto optimal_delta = ours - theirs;
  bool delta = optimal_delta.in(4) && optimal_delta.in(10) &&
               optimal_delta.elements().size() == 2;
  std::cout << "delta test: " << (delta ? "passed" : "failed") << std::endl;

  return 0;
}
