#include "Student.h"
#include <iostream>
using namespace std;

Student::Student(int id, const string& n, const string& b, int y)
    : userID(id), name(n), branch(b), year(y), skills(NULL), interests(NULL), connections(NULL) {}

int Student::getID() const { return userID; }
string Student::getName() const { return name; }
string Student::getBranch() const { return branch; }
int Student::getYear() const { return year; }
StringNode* Student::getSkills() const { return skills; }
StringNode* Student::getInterests() const { return interests; }
ConnectionNode* Student::getConnections() const { return connections; }

void Student::addSkill(const string& value) {
    if (value.empty() || hasSkill(value)) return;
    StringNode* node = new StringNode(value);
    node->next = skills;
    skills = node;
}

void Student::addInterest(const string& value) {
    if (value.empty()) return;
    for (StringNode* p = interests; p != NULL; p = p->next)
        if (p->value == value) return;
    StringNode* node = new StringNode(value);
    node->next = interests;
    interests = node;
}

bool Student::hasSkill(const string& value) const {
    for (StringNode* p = skills; p != NULL; p = p->next)
        if (p->value == value) return true;
    return false;
}

bool Student::hasConnection(int id) const {
    for (ConnectionNode* p = connections; p != NULL; p = p->next)
        if (p->studentID == id) return true;
    return false;
}

void Student::addConnection(int id) {
    if (id == userID || hasConnection(id)) return;
    ConnectionNode* node = new ConnectionNode(id);
    node->next = connections;
    connections = node;
}

void Student::displayProfile() const {
    cout << "\nID: " << userID << "\nName: " << name
         << "\nBranch: " << branch << "\nYear: " << year << "\nSkills: ";
    if (skills == NULL) cout << "None";
    for (StringNode* p = skills; p != NULL; p = p->next)
        cout << p->value << (p->next ? ", " : "");
    cout << "\nInterests: ";
    if (interests == NULL) cout << "None";
    for (StringNode* p = interests; p != NULL; p = p->next)
        cout << p->value << (p->next ? ", " : "");
    cout << "\n";
}

void Student::viewConnections() const {
    if (connections == NULL) {
        cout << "No accepted connections.\n";
        return;
    }
    cout << "Connected student IDs: ";
    for (ConnectionNode* p = connections; p != NULL; p = p->next)
        cout << p->studentID << (p->next ? " -> " : "");
    cout << "\n";
}

Student::~Student() {
    while (skills != NULL) {
        StringNode* t = skills; skills = skills->next; delete t;
    }
    while (interests != NULL) {
        StringNode* t = interests; interests = interests->next; delete t;
    }
    while (connections != NULL) {
        ConnectionNode* t = connections; connections = connections->next; delete t;
    }
}
