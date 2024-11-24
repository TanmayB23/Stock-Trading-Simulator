#ifndef DATABASE_H
#define DATABASE_H
using namespace std;
#include <string>

class Database {
public:
    Database(const string &db_name);
    bool userExists(const string &username);
    bool createUser(const string &username, const string &password);
    bool verifyLogin(const string &username, const string &password);
    double getUserBalance(const string &username);
    void updateUserBalance(const string &username, double balance);

private:
    string db_name; // Store the database name
};

#endif
