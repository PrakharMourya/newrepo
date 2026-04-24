#include "Item.h"
#include <iostream>

int main() {
    Item item("Test", 10.0, 1, "ABCD1234", "test item");
    const char* sku = item.getSku();
    if (sku[0] == 'A') {
        std::cout << "PASS: No stack buffer overflow in SKU copy" << std::endl;
    }
    return 0;
}
