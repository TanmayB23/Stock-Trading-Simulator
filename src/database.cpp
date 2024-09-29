#include "database.h"
#include <sqlite3.h>
#include <iostream>

using namespace std;

Database::Database(const string &db_name) {  // db_name parameter
    sqlite3 *db;
    int rc = sqlite3_open(db_name.c_str(), &db);
    
    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << "\n";
    } else {
        cout << "Opened database successfully.\n";

        // SQL statement to create 'users' table
        const char *sql = "CREATE TABLE IF NOT EXISTS users ("
                          "username TEXT PRIMARY KEY, "
                          "password TEXT NOT NULL, "
                          "balance REAL NOT NULL DEFAULT 0);";
        
        // Execute the SQL statement
        char *errMsg = nullptr;
        rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
        
        if (rc != SQLITE_OK) {
            cerr << "SQL error: " << errMsg << "\n";
            sqlite3_free(errMsg);
        } else {
            cout << "Table created successfully.\n";
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
