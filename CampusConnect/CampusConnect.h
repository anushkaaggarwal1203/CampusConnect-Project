#ifndef CAMPUSCONNECT_H
#define CAMPUSCONNECT_H

#include "Student.h"
#include "Chat.h"
#include <vector>

struct StudentNode {
    Student* student;
    StudentNode* next;
    StudentNode(Student* s) : student(s), next(NULL) {}
};

struct ConnectionRequest {
    int senderID, receiverID;
    ConnectionRequest* next;
    ConnectionRequest(int sender, int receiver) : senderID(sender), receiverID(receiver), next(NULL) {}
};

class CampusConnect {
private:
    StudentNode* students;
    ConnectionRequest* requestFront;
    ConnectionRequest* requestRear;
    ChatNode* chatHead;
    int studentCount;
    StudentNode* merge(StudentNode* a, StudentNode* b);
    void split(StudentNode* source, StudentNode** front, StudentNode** back);
    void mergeSort(StudentNode** head);
    bool queueHasRequest(int sender, int receiver) const;
    void enqueueRequest(int sender, int receiver);
    ConnectionRequest* dequeueRequest();
public:
    CampusConnect();
    Student* findStudentByID(int id) const;
    void registerStudent();
    bool deleteStudent(int id);
    bool removeConnection(int id1, int id2);
    void addSkillOrInterest(int currentID, bool addSkill);
    void displayAllStudents() const;
    void searchStudent() const;
    void sortStudents();
    void sendRequest(int senderID);
    void viewPendingRequests(int currentID) const;
    void processNextRequest(int currentID);
    void viewConnections(int currentID) const;
    bool sendMessage(int senderID, int receiverID, const string& text);
    void addLoadedStudent(Student* student);
    void addLoadedRequest(int sender, int receiver);
    StudentNode* getStudents() const;
    ConnectionRequest* getRequestFront() const;
    int getStudentCount() const;
    static void invalidInput();
    ~CampusConnect();
};

#endif
