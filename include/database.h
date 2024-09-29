#ifndef DATABASE_H
#define DATABASE_H

#include <string>

class Database {
public:
    Database(const std::string &db_name);
    bool userExists(const std::string &username);
    bool createUser(const std::string &username, const std::string &password);
    bool verifyLogin(const std::string &username, const std::string &password);
    double getUserBalance(const std::string &username);
    void updateUserBalance(const std::string &username, double balance);

private:
    std::string db_name; // Store the database name
};

#endif
