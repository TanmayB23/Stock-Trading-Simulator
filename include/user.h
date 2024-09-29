#ifndef USER_H
#define USER_H

#include <string>
#include <map>

class User {
public:
    std::string username;
    double balance;
    std::map<std::string, int> portfolio;

    User(const std::string &uname);
    void deposit(double amount);
    void withdraw(double amount);
    bool buyStock(const std::string &stockSymbol, int quantity, double price);
    bool sellStock(const std::string &stockSymbol, int quantity, double price);
    void displayPortfolio();
    double calculateTotalPortfolioValue();
};

#endif
