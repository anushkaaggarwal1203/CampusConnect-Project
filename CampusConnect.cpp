#include "CampusConnect.h"
#include <iostream>
#include <limits>
#include <stdexcept>
using namespace std;

CampusConnect::CampusConnect() : students(NULL), requestFront(NULL), requestRear(NULL), studentCount(0) {}

StudentNode* CampusConnect::merge(StudentNode* a, StudentNode* b) {
    if (!a) return b;
    if (!b) return a;
    StudentNode* head;
    if (a->student->getName() <= b->student->getName()) {
        head = a;
        head->next = merge(a->next, b);
    } else {
        head = b;
        head->next = merge(a, b->next);
    }
    return head;
}

void CampusConnect::split(StudentNode* source, StudentNode** front, StudentNode** back) {
    StudentNode* slow = source;
    StudentNode* fast = source->next;
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

void CampusConnect::mergeSort(StudentNode** head) {
    if (*head == NULL || (*head)->next == NULL) return;
    StudentNode *a, *b;
    split(*head, &a, &b);
    mergeSort(&a);
    mergeSort(&b);
    *head = merge(a, b);
}

bool CampusConnect::queueHasRequest(int sender, int receiver) const {
    for (ConnectionRequest* p = requestFront; p != NULL; p = p->next)
        if ((p->senderID == sender && p->receiverID == receiver) ||
            (p->senderID == receiver && p->receiverID == sender)) return true;
    return false;
}

void CampusConnect::enqueueRequest(int sender, int receiver) {
    ConnectionRequest* node = new ConnectionRequest(sender, receiver);
    if (requestRear == NULL) requestFront = requestRear = node;
    else { requestRear->next = node; requestRear = node; }
}

ConnectionRequest* CampusConnect::dequeueRequest() {
    if (requestFront == NULL) return NULL;
    ConnectionRequest* node = requestFront;
    requestFront = requestFront->next;
    if (requestFront == NULL) requestRear = NULL;
    node->next = NULL;
    return node;
}

Student* CampusConnect::findStudentByID(int id) const {
    for (StudentNode* p = students; p != NULL; p = p->next)
        if (p->student->getID() == id) return p->student;
    return NULL;
}

void CampusConnect::registerStudent() {
    int id, year;
    string name, branch;
    cout << "Student ID: ";
    if (!(cin >> id)) { invalidInput(); return; }
    if (id <= 0) { cout << "Student ID must be positive.\n"; return; }
    if (findStudentByID(id) != NULL) { cout << "This ID already exists.\n"; return; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Name: "; getline(cin, name);
    cout << "Branch: "; getline(cin, branch);
    cout << "Year (1-8): ";
    if (!(cin >> year)) { invalidInput(); return; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (name.empty() || branch.empty() || year < 1 || year > 8) {
        cout << "Name/branch cannot be empty and year must be between 1 and 8.\n";
        return;
    }
    Student* s = new Student(id, name, branch, year);
    addLoadedStudent(s);
    cout << "Student registered.\n";
}

void CampusConnect::addSkillOrInterest(int currentID, bool addSkill) {
    string value;
    Student* s = findStudentByID(currentID);
    if (s == NULL) { cout << "Student not found.\n"; return; }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << (addSkill ? "Enter skill: " : "Enter interest: ");
    getline(cin, value);
    if (value.empty()) { cout << "Value cannot be empty.\n"; return; }
    if (addSkill) s->addSkill(value); else s->addInterest(value);
    cout << "Updated successfully.\n";
}

void CampusConnect::displayAllStudents() const {
    if (students == NULL) { cout << "No students registered.\n"; return; }
    for (StudentNode* p = students; p != NULL; p = p->next) p->student->displayProfile();
}

void CampusConnect::searchStudent() const {
    int choice;
    cout << "1. Search by ID\n2. Search by branch\n3. Search by skill\nChoice: ";
    if (!(cin >> choice)) { invalidInput(); return; }
    if (choice == 1) {
        int id; cout << "Enter ID: ";
        if (!(cin >> id)) { invalidInput(); return; }
        Student* s = findStudentByID(id);
        if (s) s->displayProfile(); else cout << "Student not found.\n";
        return;
    }
    if (choice != 2 && choice != 3) { cout << "Invalid choice.\n"; return; }
    string key;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << (choice == 2 ? "Enter branch: " : "Enter skill: ");
    getline(cin, key);
    bool found = false;
    for (StudentNode* p = students; p != NULL; p = p->next) {
        bool match = (choice == 2 && p->student->getBranch() == key) ||
                     (choice == 3 && p->student->hasSkill(key));
        if (match) { p->student->displayProfile(); found = true; }
    }
    if (!found) cout << "No matching students found.\n";
}

void CampusConnect::sortStudents() {
    mergeSort(&students);
    cout << "Students sorted by name using Merge Sort.\n";
    displayAllStudents();
}

void CampusConnect::sendRequest(int sender) {
    int receiver;
    cout << "Receiver student ID: ";
    if (!(cin >> receiver)) { invalidInput(); return; }
    Student* from = findStudentByID(sender);
    Student* to = findStudentByID(receiver);
    if (!from || !to) { cout << "Student ID not found.\n"; return; }
    if (sender == receiver) { cout << "You cannot connect with yourself.\n"; return; }
    if (from->hasConnection(receiver)) { cout << "Already connected.\n"; return; }
    if (queueHasRequest(sender, receiver)) { cout << "A request already exists between these students.\n"; return; }
    enqueueRequest(sender, receiver);
    cout << "Request added to the pending-request queue.\n";
}

void CampusConnect::viewPendingRequests(int currentID) const {
    bool found = false;
    cout << "Requests received by you (front to rear):\n";
    for (ConnectionRequest* p = requestFront; p != NULL; p = p->next) {
        if (p->receiverID == currentID) {
            cout << p->senderID << " -> " << p->receiverID << "\n";
            found = true;
        }
    }
    if (!found) cout << "No pending requests for you.\n";
}

void CampusConnect::processNextRequest(int currentID) {
    ConnectionRequest* previous = NULL;
    ConnectionRequest* req = requestFront;
    while (req != NULL && req->receiverID != currentID) {
        previous = req;
        req = req->next;
    }
    if (req == NULL) { cout << "No pending requests for you.\n"; return; }
    if (previous == NULL) requestFront = req->next;
    else previous->next = req->next;
    if (requestRear == req) requestRear = previous;
    cout << "Request from student " << req->senderID << " to you.\n";
    cout << "1. Accept\n2. Reject\nChoice: ";
    int choice;
    if (!(cin >> choice)) { invalidInput(); delete req; return; }
    if (choice == 1) {
        Student* sender = findStudentByID(req->senderID);
        Student* receiver = findStudentByID(currentID);
        if (sender && receiver) {
            sender->addConnection(receiver->getID());
            receiver->addConnection(sender->getID());
            cout << "Request accepted. Both students are connected.\n";
        } else cout << "Request could not be accepted because a student record is missing.\n";
    } else if (choice == 2) cout << "Request rejected.\n";
    else cout << "Invalid choice. Request removed from queue.\n";
    delete req;
}

void CampusConnect::viewConnections(int currentID) const {
    Student* s = findStudentByID(currentID);
    if (!s) { cout << "Student not found.\n"; return; }
    s->viewConnections();
}

void CampusConnect::addLoadedStudent(Student* student) {
    if (student == NULL) throw invalid_argument("Cannot add an empty student record.");
    if (findStudentByID(student->getID()) != NULL)
        throw runtime_error("Duplicate student ID.");
    StudentNode* node = new StudentNode(student);
    node->next = students;
    students = node;
    studentCount++;
}

void CampusConnect::addLoadedRequest(int sender, int receiver) {
    Student* from = findStudentByID(sender);
    Student* to = findStudentByID(receiver);
    if (from && to && sender != receiver && !from->hasConnection(receiver) && !queueHasRequest(sender, receiver))
        enqueueRequest(sender, receiver);
}

StudentNode* CampusConnect::getStudents() const { return students; }
ConnectionRequest* CampusConnect::getRequestFront() const { return requestFront; }
int CampusConnect::getStudentCount() const { return studentCount; }

void CampusConnect::invalidInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input. Please enter the correct type of value.\n";
}

CampusConnect::~CampusConnect() {
    while (requestFront != NULL) {
        ConnectionRequest* t = requestFront; requestFront = requestFront->next; delete t;
    }
    while (students != NULL) {
        StudentNode* t = students; students = students->next; delete t->student; delete t;
    }
}
