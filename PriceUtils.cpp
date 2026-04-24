#include "PriceUtils.h"

double applyDiscount(double price, double percent) {
    return price * (1.0 - percent / 100.0);
}

double applyTax(double price, double rate) {
    return price * (1.0 + rate / 100.0);
}

double itemSubtotal(const Item& item) {
    return item.getPrice() * item.getQuantity();
}

std::size_t itemStorageSize() {
    return sizeof(Item);
}
