#ifndef STOCK_H
#define STOCK_H
#include <string>
using namespace std;

class Stock {
public:
    string symbol;
    double price;

    Stock(const string &symb);
    void updatePrice();  // Function to update stock price from API
};

#endif
