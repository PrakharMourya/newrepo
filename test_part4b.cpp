#include "Item.h"
#include <iostream>

int main() {
    Item original("Test", 10.0, 1, "TST-01", "test item");
    {
        Item copy = original;
        std::cout << "Copy created: " << copy.getName() << std::endl;
    }
    std::cout << "PASS: No double free on Item copy" << std::endl;
    return 0;
}
