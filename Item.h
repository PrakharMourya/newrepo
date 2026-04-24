#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

class Item {
    std::string name_;
    double price_;
    int quantity_;
    char sku_[8];
    char* description_;

public:
    Item(const std::string& name, double price, int quantity,
         const std::string& sku, const std::string& desc);

    Item(const Item& other);
    Item& operator=(const Item& other);

    ~Item();

    std::string getName() const;
    double getPrice() const;
    int getQuantity() const;
    const char* getSku() const;
    const char* getDescription() const;

    friend std::ostream& operator<<(std::ostream& os, const Item& item);
};

#endif
