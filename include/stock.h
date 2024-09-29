#ifndef STOCK_H
#define STOCK_H

#include <string>

class Stock {
public:
    std::string symbol;
    double price;

    Stock(const std::string &symb);
    void updatePrice();  // Function to update stock price from API
};

#endif
