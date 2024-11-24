#include "stock.h"
#include "user.h"
#include "database.h"
#include <iostream>
#include <string>

using namespace std;

void showMenu() {
    cout << "Select an option:\n";
    cout << "1. Deposit Money\n";
    cout << "2. Withdraw Money\n";
    cout << "3. Buy Stock\n";
    cout << "4. Sell Stock\n";
    cout << "5. Check Balance\n";
    cout << "6. Show Portfolio\n";
    cout << "7. Exit\n";
}

int main() {
    Database db("data/users.db");

    cout << "Welcome to the Stock Trading Simulator!\n";
    cout << "1. Login\n2. Create Account\n";
    int choice;
    cin >> choice;

    string username, password;
    if (choice == 1) {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        if (db.verifyLogin(username, password)) {
            cout << "Login successful!\n";
            User user(username);
            int option = 0;

            while (option != 7) {
                showMenu();
                cin >> option;

                switch (option) {
                    case 1: {
                        double depositAmount;
                        cout << "Enter amount to deposit: ";
                        cin >> depositAmount;
                        double currentBalance = db.getUserBalance(username);
                        db.updateUserBalance(username, currentBalance + depositAmount);
                        cout << "Deposit successful! New balance: ₹" << db.getUserBalance(username) << "\n";
                        break;
                    }
                    case 2: {
                        double withdrawAmount;
                        cout << "Enter amount to withdraw: ";
                        cin >> withdrawAmount;
                        double currentBalance = db.getUserBalance(username);
                        if (withdrawAmount > currentBalance) {
                            cout << "Insufficient balance!\n";
                        } else {
                            db.updateUserBalance(username, currentBalance - withdrawAmount);
                            cout << "Withdrawal successful! New balance: ₹" << db.getUserBalance(username) << "\n";
                        }
                        break;
                    }
                    case 3: {
                        string stockSymbol;
                        int quantity;
                        cout << "Enter stock symbol: ";
                        cin >> stockSymbol;
                        cout << "Enter quantity to buy: ";
                        cin >> quantity;

                        Stock stock(stockSymbol);
                        stock.updatePrice();  // Fetch the current stock price
                        double price = stock.price;  // Get the fetched price
                        double totalCost = price * quantity;

                        double currentBalance = db.getUserBalance(username);
                        if (totalCost > currentBalance) {
                            cout << "Insufficient balance to buy stocks.\n";
                        } else {
                            db.updateUserBalance(username, currentBalance - totalCost);
                            user.buyStock(stockSymbol, quantity, totalCost);
                            db.updatePortfolio(username, stockSymbol, quantity);
                            cout << "Bought " << quantity << " shares of " << stockSymbol << " at ₹" << price << " each.\n";
                        }
                        break;
                    }
                    case 4: {
                        string stockSymbol;
                        int quantity;
                        cout << "Enter stock symbol: ";
                        cin >> stockSymbol;
                        cout << "Enter quantity to sell: ";
                        cin >> quantity;

                        Stock stock(stockSymbol);
                        stock.updatePrice();  // Fetch the current stock price
                        double price = stock.price;  // Get the fetched price
                        double totalEarnings = price * quantity;

                        if (user.sellStock(stockSymbol, quantity, totalEarnings)) {
                            double currentBalance = db.getUserBalance(username);
                            db.updateUserBalance(username, currentBalance + totalEarnings);
                            db.updatePortfolio(username, stockSymbol, -quantity);
                            cout << "Sold " << quantity << " shares of " << stockSymbol << " at ₹" << price << " each.\n";
                        } else {
                            cout << "You don't have enough shares to sell.\n";
                        }
                        break;
                    }
                    case 5: {
                        cout << "Your current balance: ₹" << db.getUserBalance(username) << "\n";
                        break;
                    }
                    case 6: {
                        map<string, int> portfolio = db.getPortfolio(username);
                        cout << "Your portfolio:\n";
                        for (const auto &entry : portfolio) {
                            cout << entry.first << ": " << entry.second << " shares\n";
                        }
                        break;
                    }
                    case 7:
                        cout << "Exiting...\n";
                        break;
                    default:
                        cout << "Invalid option. Please try again.\n";
                        break;
                }
            }

        } else {
            cout << "Invalid credentials.\n";
        }
    } else if (choice == 2) {
        cout << "Create new account\n";
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        if (db.createUser(username, password)) {
            cout << "Account created successfully!\n";
        } else {
            cout << "Error creating account.\n";
        }
    }

    return 0;
}
