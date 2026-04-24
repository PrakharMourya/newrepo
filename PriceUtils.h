#ifndef PRICE_UTILS_H
#define PRICE_UTILS_H

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
std::string formatPrice(double price);

template<typename T>
T clampValue(T val, T low, T high) {
    if (val < low) return low;
    if (val > high) return high;
    return val;
}

#endif
