#ifndef PRICEUTILS_H
#define PRICEUTILS_H

#include <string>
#include <sstream>
#include <iomanip>
#include <cstddef>
#include "Item.h"

struct DiscountConfig {
    double percent;
    double minimum;
};

double applyDiscount(double price, double percent);
double applyTax(double price, double rate);
double itemSubtotal(const Item& item);
std::size_t itemStorageSize();

inline std::string formatPrice(double price) {
    std::ostringstream oss;
    oss << "$" << std::fixed << std::setprecision(2) << price;
    return oss.str();
}

template<typename T>
T clampValue(T val, T low, T high) {
    if (val < low) return low;
    if (val > high) return high;
    return val;
}

#endif
