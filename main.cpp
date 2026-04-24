#include "Item.h"
#include "Warehouse.h"
#include "Report.h"
#include "PriceUtils.h"
#include <iostream>

int main() {
    Warehouse wh("Central Storage");

    Item laptop("Laptop", 999.99, 5, "LAP-0010", "High-performance laptop");
    Item mouse("Mouse", 29.99, 50, "MOU-0020", "Wireless optical mouse");
    Item keyboard("Keyboard", 79.99, 30, "KEY-0030", "Mechanical keyboard");

    wh.addItem(laptop);
    wh.addItem(mouse);
    wh.addItem(keyboard);

    generateReport(wh, std::cout);

    std::cout << std::endl;
    std::cout << "Summary: " << generateSummary(wh) << std::endl;

    double discounted = applyDiscount(999.99, 10.0);
    std::cout << "Laptop with 10% discount: " << formatPrice(discounted) << std::endl;

    int clamped = clampValue(150, 0, 100);
    std::cout << "Clamped value: " << clamped << std::endl;

    return 0;
}
