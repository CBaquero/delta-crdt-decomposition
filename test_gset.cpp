#include <iostream>
#include "gset.h"

int main() {
    gset<int> set1, set2, set3, setMain;

    // Adding to set1
    set1.add(5);
    set1.add(10);
    set1.add(15);

    // Adding to set2
    set2.add(20);
    set2.add(25);

    // Adding to set3
    set3.add(30);
    set3.add(35);

    // Checking if elements are in the sets
    std::cout << "set1 contains 10: " << set1.in(10) << std::endl; // Expected output: 1 (true)
    std::cout << "set1 contains 20: " << set1.in(20) << std::endl; // Expected output: 0 (false)

    // Joining multiple sets
    setMain.join(set1, set2, set3);

    // Checking if elements are in the main set after join
    std::cout << "setMain contains 5: " << setMain.in(5) << std::endl;   // Expected output: 1 (true)
    std::cout << "setMain contains 25: " << setMain.in(25) << std::endl; // Expected output: 1 (true)
    std::cout << "setMain contains 30: " << setMain.in(30) << std::endl; // Expected output: 1 (true)

    std::vector<gset<int>> setsToJoin = {set1, set2, set3};
    setMain.join(setsToJoin);

    // Checking if elements are in the main set after join
    std::cout << "setMain contains 5: " << setMain.in(5) << std::endl;   // Expected output: 1 (true)
    std::cout << "setMain contains 25: " << setMain.in(25) << std::endl; // Expected output: 1 (true)
    std::cout << "setMain contains 35: " << setMain.in(35) << std::endl; // Expected output: 1 (true)

    std::vector<gset<int>> setsToJoin2 = setMain.split();
    gset<int> rejoinedSet;
    rejoinedSet.join(setsToJoin2);

    std::cout << setMain << std::endl;
    std::cout << rejoinedSet << std::endl;

    std::cout << rejoinedSet - set3 << std::endl;
    

    return 0;
}

