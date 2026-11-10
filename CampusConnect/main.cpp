#include "CampusConnect.h"
#include "FileHandler.h"
#include "Auth.h"
#include <iostream>
#include <exception>
using namespace std;

int main() {
    CampusConnect app;
    Auth auth;
    try {
        FileHandler::load(app);
        auth.load();
        cout << "CampusConnect data loaded.\n";
    } catch (const exception& e) {
        cerr << "Loading error: " << e.what() << "\n";
        cerr << "Check the saved text files before continuing.\n";
    }

    bool running = true;
    while (running) {
        cout << "\n========== CAMPUSCONNECT =========="
             << "\n1. Create account / register student"
             << "\n2. Login"
             << "\n0. Save and exit"
             << "\nEnter choice: ";
        int choice;
        if (!(cin >> choice)) {
            CampusConnect::invalidInput();
            if (cin.eof()) break;
            continue;
        }
        try {
            if (choice == 1) {
                auth.registerAccount(app);
            } else if (choice == 2) {
                int currentID = auth.login();
                if (currentID == -1) continue;
                if (app.findStudentByID(currentID) == NULL) {
                    cout << "No student profile is linked to this account.\n";
                    continue;
                }
                bool loggedIn = true;
                while (loggedIn) {
                    cout << "\n------ STUDENT MENU (ID: " << currentID << ") ------\n"
                         << "1. View my profile\n"
                         << "2. Add skill\n"
                         << "3. Add interest\n"
                         << "4. Display all student profiles\n"
                         << "5. Search students\n"
                         << "6. Sort students by name (Merge Sort)\n"
                         << "7. Send connection request\n"
                         << "8. View my pending requests\n"
                         << "9. Process a request sent to me\n"
                         << "10. View my accepted connections\n"
                         << "11. Logout\n"
                         << "Enter choice: ";
                    int option;
                    if (!(cin >> option)) {
                        CampusConnect::invalidInput();
                        if (cin.eof()) { loggedIn = false; running = false; }
                        continue;
                    }
                    try {
                        switch (option) {
                            case 1: app.findStudentByID(currentID)->displayProfile(); break;
                            case 2: app.addSkillOrInterest(currentID, true); break;
                            case 3: app.addSkillOrInterest(currentID, false); break;
                            case 4: app.displayAllStudents(); break;
                            case 5: app.searchStudent(); break;
                            case 6: app.sortStudents(); break;
                            case 7: app.sendRequest(currentID); break;
                            case 8: app.viewPendingRequests(currentID); break;
                            case 9: app.processNextRequest(currentID); break;
                            case 10: app.viewConnections(currentID); break;
                            case 11: loggedIn = false; cout << "Logged out.\n"; break;
                            default: cout << "Invalid choice.\n";
                        }
                    } catch (const exception& e) {
                        cout << "Operation failed: " << e.what() << "\n";
                    }
                }
            } else if (choice == 0) {
                running = false;
            } else {
                cout << "Invalid choice.\n";
            }
        } catch (const exception& e) {
            cout << "Operation failed: " << e.what() << "\n";
        }
    }

    try {
        FileHandler::save(app);
        auth.save();
        cout << "Data saved. Exiting CampusConnect.\n";
    } catch (const exception& e) {
        cerr << "Saving error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
