#include "Item.h"
#include <cstring>

Item::Item(const std::string& name, double price, int quantity,
           const std::string& sku, const std::string& desc)
    : name_(name), price_(price), quantity_(quantity) {

    std::strncpy(sku_, sku.c_str(), 7);
    sku_[7] = '\0';

    description_ = new char[desc.size() + 1];
    std::strcpy(description_, desc.c_str());
}

Item::Item(const Item& other)
    : name_(other.name_), price_(other.price_), quantity_(other.quantity_) {

    std::memcpy(sku_, other.sku_, 8);
    description_ = new char[std::strlen(other.description_) + 1];
    std::strcpy(description_, other.description_);
}

Item& Item::operator=(const Item& other) {
    if (this != &other) {
        name_ = other.name_;
        price_ = other.price_;
        quantity_ = other.quantity_;

        std::memcpy(sku_, other.sku_, 8);

        delete[] description_;
        description_ = new char[std::strlen(other.description_) + 1];
        std::strcpy(description_, other.description_);
    }
    return *this;
}

Item::~Item() {
    delete[] description_;
}

std::string Item::getName() const { return name_; }
double Item::getPrice() const { return price_; }
int Item::getQuantity() const { return quantity_; }
const char* Item::getSku() const { return sku_; }
const char* Item::getDescription() const { return description_; }

std::ostream& operator<<(std::ostream& os, const Item& item) {
    os << item.name_ << " [" << item.sku_ << "] $" << item.price_
       << " x" << item.quantity_;
    return os;
}
