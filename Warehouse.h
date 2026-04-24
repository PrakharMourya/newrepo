#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include <string>
#include <vector>
#include <iostream>
#include "Item.h"

class Warehouse {
    std::string name_;
    std::vector<Item> items_;

public:
    Warehouse(const std::string& name);
    void addItem(const Item& item);
    const Item& getItem(int index) const;
    int itemCount() const;
    double totalValue() const;
    const std::string& getName() const;

    friend std::ostream& operator<<(std::ostream& os, const Warehouse& wh);
};

#endif
