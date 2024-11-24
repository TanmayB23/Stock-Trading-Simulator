#include "database.h"
#include <sqlite3.h>
#include <iostream>
#include <map>

using namespace std;

Database::Database(const string &db_name) : db_name(db_name) {
    sqlite3 *db;
    int rc = sqlite3_open(db_name.c_str(), &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << "\n";
    } else {
        cout << "Opened database successfully.\n";

        // SQL to create 'users' table
        const char *sql_users = "CREATE TABLE IF NOT EXISTS users ("
                                "username TEXT PRIMARY KEY, "
                                "password TEXT NOT NULL, "
                                "balance REAL NOT NULL DEFAULT 0);";

        // SQL to create 'portfolio' table
        const char *sql_portfolio = "CREATE TABLE IF NOT EXISTS portfolio ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            "username TEXT NOT NULL, "
                            "stock_name TEXT NOT NULL, "
                            "quantity INTEGER NOT NULL, "
                            "price REAL NOT NULL, " // Adding price column
                            "FOREIGN KEY(username) REFERENCES users(username));";

        // Execute table creation queries
        char *errMsg = nullptr;
        rc = sqlite3_exec(db, sql_users, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "SQL error: " << errMsg << "\n";
            sqlite3_free(errMsg);
        }

        rc = sqlite3_exec(db, sql_portfolio, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            cerr << "SQL error: " << errMsg << "\n";
            sqlite3_free(errMsg);
        } else {
            cout << "Tables created successfully.\n";
        }
    }

    sqlite3_close(db);
}

bool Database::userExists(const string &username) {
    sqlite3 *db;
    sqlite3_open(db_name.c_str(), &db);

    string query = "SELECT COUNT(*) FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return true;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return false;
}

bool Database::createUser(const string &username, const string &password) {
    sqlite3 *db;
    sqlite3_open(db_name.c_str(), &db);

    string query = "INSERT INTO users (username, password, balance) VALUES (?, ?, 0);";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return true;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return false;
}

bool Database::verifyLogin(const string &username, const string &password) {
    sqlite3 *db;
    sqlite3_open(db_name.c_str(), &db);

    string query = "SELECT COUNT(*) FROM users WHERE username = ? AND password = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return true;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return false;
}

double Database::getUserBalance(const string &username) {
    sqlite3 *db;
    sqlite3_open(db_name.c_str(), &db);

    string query = "SELECT balance FROM users WHERE username = ?;";
    sqlite3_stmt *stmt;
    double balance = 0.0;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            balance = sqlite3_column_double(stmt, 0);
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return balance;
}

void Database::updateUserBalance(const string &username, double balance) {
    sqlite3 *db;
    sqlite3_open(db_name.c_str(), &db);

    string query = "UPDATE users SET balance = ? WHERE username = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

    if (rc == SQLITE_OK) {
        sqlite3_bind_double(stmt, 1, balance);
        sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void Database::updatePortfolio(const string &username, const string &stockName, int quantity, double price) {
    sqlite3 *db;
    sqlite3_open(db_name.c_str(), &db);

    string query = "SELECT COUNT(*) FROM portfolio WHERE username = ? AND stock_name = ?;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    int count = 0;

    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, stockName.c_str(), -1, SQLITE_STATIC);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);

    if (count > 0) {
        query = "UPDATE portfolio SET quantity = quantity + ?, price = ? WHERE username = ? AND stock_name = ?;";
    } else {
        query = "INSERT INTO portfolio (username, stock_name, quantity, price) VALUES (?, ?, ?, ?);";
    }

    rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        if (count > 0) {
            sqlite3_bind_int(stmt, 1, quantity);
            sqlite3_bind_double(stmt, 2, price);
            sqlite3_bind_text(stmt, 3, username.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, stockName.c_str(), -1, SQLITE_STATIC);
        } else {
            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, stockName.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 3, quantity);
            sqlite3_bind_double(stmt, 4, price);
        }
        sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

map<string, int> Database::getPortfolio(const string &username) {
    sqlite3 *db;
    sqlite3_open(db_name.c_str(), &db);

    string query = "SELECT stock_name, quantity FROM portfolio WHERE username = ?;";
    sqlite3_stmt *stmt;
    map<string, int> portfolio;

    int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string stockName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
            int quantity = sqlite3_column_int(stmt, 1);
            portfolio[stockName] = quantity;
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return portfolio;
}
