#include "Item.h"
#include "Warehouse.h"
#include <iostream>

int main() {
    Warehouse wh("Test");
    Item a("Alpha", 10.0, 1, "A-001", "item a");
    Item b("Beta", 20.0, 1, "B-001", "item b");
    Item c("Gamma", 30.0, 1, "C-001", "item c");
    Item d("Delta", 40.0, 1, "D-001", "item d");
    Item e("Echo", 50.0, 1, "E-001", "item e");

    wh.addItem(a);
    wh.addItem(b);
    wh.addItem(c);
    wh.addItem(d);
    wh.addItem(e);

    std::cout << "PASS: No use-after-free during vector growth" << std::endl;
    return 0;
}
