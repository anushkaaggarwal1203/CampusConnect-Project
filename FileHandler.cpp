#include "FileHandler.h"
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <limits>
using namespace std;

void FileHandler::save(const CampusConnect& app, const string& studentFile, const string& requestFile) {
    ofstream out(studentFile.c_str());
    if (!out) throw runtime_error("Could not open " + studentFile + " for writing.");

    for (StudentNode* p = app.getStudents(); p != NULL; p = p->next) {
        Student* s = p->student;
        out << s->getID() << ' ' << s->getYear() << ' '
            << quoted(s->getName()) << ' ' << quoted(s->getBranch()) << ' ';

        int count = 0;
        for (StringNode* q = s->getSkills(); q != NULL; q = q->next) count++;
        out << count << ' ';
        for (StringNode* q = s->getSkills(); q != NULL; q = q->next) out << quoted(q->value) << ' ';

        count = 0;
        for (StringNode* q = s->getInterests(); q != NULL; q = q->next) count++;
        out << count << ' ';
        for (StringNode* q = s->getInterests(); q != NULL; q = q->next) out << quoted(q->value) << ' ';

        count = 0;
        for (ConnectionNode* q = s->getConnections(); q != NULL; q = q->next) count++;
        out << count << ' ';
        for (ConnectionNode* q = s->getConnections(); q != NULL; q = q->next) out << q->studentID << ' ';
        out << '\n';
    }
    if (!out) throw runtime_error("Error while writing student records.");

    ofstream reqOut(requestFile.c_str());
    if (!reqOut) throw runtime_error("Could not open " + requestFile + " for writing.");
    for (ConnectionRequest* p = app.getRequestFront(); p != NULL; p = p->next)
        reqOut << p->senderID << ' ' << p->receiverID << '\n';
    if (!reqOut) throw runtime_error("Error while writing connection requests.");
}

void FileHandler::load(CampusConnect& app, const string& studentFile, const string& requestFile) {
    ifstream in(studentFile.c_str());
    if (in) {
        while (true) {
            int id, year, count;
            string name, branch;
            if (!(in >> id)) {
                if (in.eof()) break;
                throw runtime_error("Invalid data in " + studentFile + ".");
            }
            if (!(in >> year >> quoted(name) >> quoted(branch) >> count) || count < 0 || count > 10000)
                throw runtime_error("Invalid student data in " + studentFile + ".");
            Student* s = new Student(id, name, branch, year);
            try {
                for (int i = 0; i < count; i++) {
                    string value;
                    if (!(in >> quoted(value))) throw runtime_error("Invalid skills data in " + studentFile + ".");
                    s->addSkill(value);
                }
                if (!(in >> count) || count < 0 || count > 10000) throw runtime_error("Invalid interests data in " + studentFile + ".");
                for (int i = 0; i < count; i++) {
                    string value;
                    if (!(in >> quoted(value))) throw runtime_error("Invalid interests data in " + studentFile + ".");
                    s->addInterest(value);
                }
                if (!(in >> count) || count < 0 || count > 100000) throw runtime_error("Invalid connections data in " + studentFile + ".");
                for (int i = 0; i < count; i++) {
                    int connectedID;
                    if (!(in >> connectedID)) throw runtime_error("Invalid connection ID in " + studentFile + ".");
                    s->addConnection(connectedID);
                }
                app.addLoadedStudent(s);
            } catch (...) {
                delete s;
                throw;
            }
        }
    }
    // If the file does not exist on the first run, start with an empty student list.

    ifstream reqIn(requestFile.c_str());
    if (reqIn) {
        int sender, receiver;
        while (reqIn >> sender >> receiver) app.addLoadedRequest(sender, receiver);
        if (!reqIn.eof()) throw runtime_error("Invalid request data in " + requestFile + ".");
    }
}
