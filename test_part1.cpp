#include "Item.h"
#include "PriceUtils.h"
#include "Warehouse.h"
#include "Report.h"

#include "Item.h"
#include "PriceUtils.h"
#include "Warehouse.h"
#include "Report.h"

#include <iostream>

int passed = 0;
int failed = 0;

int main() {
    std::cout << "PASS: All headers compile without errors" << std::endl;
    passed++;
    std::cout << "PASS: Double inclusion handled (header guards work)" << std::endl;
    passed++;
    std::cout << "PASS: No circular dependency errors" << std::endl;
    passed++;

    std::cout << std::endl;
    std::cout << "Part 1 results: " << passed << " passed, " << failed << " failed" << std::endl;
    return 0;
}
