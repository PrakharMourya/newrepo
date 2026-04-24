#include "Item.h"
#include "PriceUtils.h"
#include "Warehouse.h"
#include "Report.h"
#include <iostream>
#include <sstream>

int passed = 0;
int failed = 0;

void check(bool condition, const std::string& label) {
    if (condition) {
        std::cout << "PASS: " << label << std::endl;
        passed++;
    } else {
        std::cout << "FAIL: " << label << std::endl;
        failed++;
    }
}

int main() {
    std::string price = formatPrice(99.99);
    check(price == "$99.99",
          "formatPrice links correctly (no multiple definition)");

    Warehouse wh("Link Test");
    Item item("Widget", 5.0, 10, "WDG-001", "A widget");
    wh.addItem(item);
    std::string summary = generateSummary(wh);
    check(!summary.empty(),
          "generateSummary links correctly (Report.o in Makefile)");

    int clamped = clampValue(150, 0, 100);
    check(clamped == 100,
          "clampValue<int> links correctly (template in header)");

    double clampedD = clampValue(3.5, 1.0, 2.0);
    check(clampedD == 2.0,
          "clampValue<double> links correctly (template in header)");

    std::cout << std::endl;
    std::cout << "Part 3 results: " << passed << " passed, " << failed << " failed" << std::endl;
    return (failed > 0) ? 1 : 0;
}
