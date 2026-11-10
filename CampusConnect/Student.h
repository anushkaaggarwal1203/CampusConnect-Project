#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using namespace std;

struct StringNode {
    string value;
    StringNode* next;
    StringNode(const string& v) : value(v), next(NULL) {}
};

struct ConnectionNode {
    int studentID;
    ConnectionNode* next;
    ConnectionNode(int id) : studentID(id), next(NULL) {}
};

class Student {
private:
    int userID;
    string name, branch;
    int year;
    StringNode* skills;
    StringNode* interests;
    ConnectionNode* connections;
public:
    Student(int id, const string& n, const string& b, int y);
    int getID() const;
    string getName() const;
    string getBranch() const;
    int getYear() const;
    StringNode* getSkills() const;
    StringNode* getInterests() const;
    ConnectionNode* getConnections() const;
    void addSkill(const string& value);
    void addInterest(const string& value);
    bool hasSkill(const string& value) const;
    bool hasConnection(int id) const;
    void addConnection(int id);
    void removeConnection(int id);
    void displayProfile() const;
    void viewConnections() const;
    ~Student();
};

#endif
