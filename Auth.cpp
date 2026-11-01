#include "Auth.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <stdexcept>
using namespace std;

Auth::Auth() : accounts(NULL) {}

Auth::AccountNode* Auth::findAccount(int studentID) const {
    for (AccountNode* p = accounts; p != NULL; p = p->next)
        if (p->studentID == studentID) return p;
    return NULL;
}

void Auth::load(const string& fileName) {
    ifstream in(fileName.c_str());
    if (!in) return;
    int id;
    string password;
    while (in >> id >> quoted(password)) {
        if (id <= 0 || password.empty()) throw runtime_error("Invalid account record in " + fileName + ".");
        if (findAccount(id) == NULL) {
            AccountNode* node = new AccountNode(id, password);
            node->next = accounts;
            accounts = node;
        }
    }
    if (!in.eof()) throw runtime_error("Could not read account data in " + fileName + ".");
}

void Auth::save(const string& fileName) const {
    ofstream out(fileName.c_str());
    if (!out) throw runtime_error("Could not open " + fileName + " for writing.");
    for (AccountNode* p = accounts; p != NULL; p = p->next)
        out << p->studentID << ' ' << quoted(p->password) << '\n';
    if (!out) throw runtime_error("Error while saving account data.");
}

bool Auth::registerAccount(CampusConnect& app) {
    int id;
    cout << "Choose a positive student ID: ";
    if (!(cin >> id)) { CampusConnect::invalidInput(); return false; }
    if (id <= 0) { cout << "Student ID must be positive.\n"; return false; }
    if (findAccount(id) != NULL) { cout << "An account already exists for this ID. Please log in.\n"; return false; }

    Student* student = app.findStudentByID(id);
    if (student == NULL) {
        string name, branch;
        int year;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Name: "; getline(cin, name);
        cout << "Branch: "; getline(cin, branch);
        cout << "Year (1-8): ";
        if (!(cin >> year)) { CampusConnect::invalidInput(); return false; }
        if (name.empty() || branch.empty() || year < 1 || year > 8) {
            cout << "Name/branch cannot be empty and year must be between 1 and 8.\n";
            return false;
        }
        student = new Student(id, name, branch, year);
        try { app.addLoadedStudent(student); }
        catch (...) { delete student; throw; }
    } else {
        cout << "An existing profile was found for this ID. A login will be created for it.\n";
    }

    string password;
    cout << "Create password (no spaces): ";
    if (!(cin >> password)) { CampusConnect::invalidInput(); return false; }
    if (password.size() < 4) { cout << "Password must contain at least 4 characters.\n"; return false; }
    AccountNode* node = new AccountNode(id, password);
    node->next = accounts;
    accounts = node;
    cout << "Account created successfully. Please log in.\n";
    return true;
}

int Auth::login() const {
    int id;
    string password;
    cout << "Student ID: ";
    if (!(cin >> id)) { CampusConnect::invalidInput(); return -1; }
    cout << "Password: ";
    if (!(cin >> password)) { CampusConnect::invalidInput(); return -1; }
    AccountNode* account = findAccount(id);
    if (account == NULL || account->password != password) {
        cout << "Incorrect student ID or password.\n";
        return -1;
    }
    cout << "Login successful. Welcome!\n";
    return id;
}

Auth::~Auth() {
    while (accounts != NULL) {
        AccountNode* temp = accounts;
        accounts = accounts->next;
        delete temp;
    }
}
