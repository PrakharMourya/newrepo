#include "Warehouse.h"

Warehouse::Warehouse(const std::string& name) : name_(name) {}

void Warehouse::addItem(const Item& item) {
    double firstPrice = 0.0;

    if (!items_.empty()) {
        firstPrice = items_[0].getPrice();
    }

    items_.push_back(item);

    if (!items_.empty()) {
        (void)firstPrice; 
    }
}

const Item& Warehouse::getItem(int index) const {
    return items_[index];
}

int Warehouse::itemCount() const {
    return static_cast<int>(items_.size());
}

double Warehouse::totalValue() const {
    double total = 0.0;
    for (int i = 0; i < static_cast<int>(items_.size()); i++) {
        total += items_[i].getPrice() * items_[i].getQuantity();
    }
    return total;
}

const std::string& Warehouse::getName() const {
    return name_;
}

std::ostream& operator<<(std::ostream& os, const Warehouse& wh) {
    os << "Warehouse: " << wh.name_ << " (" << wh.items_.size() << " items)";
    return os;
}
