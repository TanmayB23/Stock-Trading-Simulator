#include "user.h"
#include <iostream>

using namespace std;

User::User(const string &uname) : username(uname), balance(0.0) {}

void User::deposit(double amount) {
    balance += amount;
    cout << "Deposit successful. New balance: " << balance << "\n";
}

void User::withdraw(double amount) {
    if (amount <= balance) {
        balance -= amount;
        cout << "Withdrawal successful. New balance: " << balance << "\n";
    } else {
        cout << "Insufficient balance.\n";
    }
}

bool User::buyStock(const string &stockSymbol, int quantity, double price) {
    double cost = quantity * price;
    if (balance >= cost) {
        balance -= cost;
        portfolio[stockSymbol] += quantity;
        cout << "Bought " << quantity << " shares of " << stockSymbol << "\n";
        return true;
    } else {
        cout << "Insufficient balance to buy stocks.\n";
        return false;
    }
}

bool User::sellStock(const string &stockSymbol, int quantity, double price) {
    if (portfolio[stockSymbol] >= quantity) {
        portfolio[stockSymbol] -= quantity;
        balance += quantity * price;
        cout << "Sold " << quantity << " shares of " << stockSymbol << "\n";
        return true;
    } else {
        cout << "Not enough shares to sell.\n";
        return false;
    }
}


void User::displayPortfolio() {
    if (portfolio.empty()) {
        cout << "Your portfolio is empty.\n";
    } else {
        cout << "Portfolio:\n";
        for (auto &stock : portfolio) {
            cout << stock.first << ": " << stock.second << " shares\n";
        }
    }
}

double User::calculateTotalPortfolioValue() {
    double totalValue = 0.0;
    for (auto &stock : portfolio) {
        // Assume latest prices have been updated
        totalValue += stock.second * 100; // Replace with actual stock price from API
    }
    return totalValue;
}
