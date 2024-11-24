#ifndef USER_H
#define USER_H

#include <string>
#include <map>
using namespace std;

class User {
public:
    string username;
    double balance;
    map<string, int> portfolio;

    User(const string &uname);
    void deposit(double amount);
    void withdraw(double amount);
    bool buyStock(const string &stockSymbol, int quantity, double price);
    bool sellStock(const string &stockSymbol, int quantity, double price);
    void displayPortfolio();
    double calculateTotalPortfolioValue();
};

#endif
