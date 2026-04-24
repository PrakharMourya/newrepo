#include "PriceUtils.h"
#include "Item.h"

double applyDiscount(double price, double percent) {
    return price * (1.0 - percent / 100.0);
}

double applyTax(double price, double rate) {
    return price * (1.0 + rate / 100.0);
}

std::string formatPrice(double price) {
    std::ostringstream oss;
    oss << "$" << std::fixed << std::setprecision(2) << price;
    return oss.str();
}

double itemSubtotal(const Item& item) {
    return item.getPrice() * item.getQuantity();
}

std::size_t itemStorageSize() {
    return sizeof(Item);
}
