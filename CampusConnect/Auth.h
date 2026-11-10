#ifndef AUTH_H
#define AUTH_H

#include "CampusConnect.h"
#include <string>

class Auth {
private:
    struct AccountNode {
        int studentID;
        std::string password;
        AccountNode* next;
        AccountNode(int id, const std::string& pass) : studentID(id), password(pass), next(NULL) {}
    };
    AccountNode* accounts;
    AccountNode* findAccount(int studentID) const;
public:
    Auth();
    ~Auth();
    void load(const std::string& fileName = "accounts.txt");
    void save(const std::string& fileName = "accounts.txt") const;
    bool registerAccount(CampusConnect& app);
    int login() const;
};

#endif
