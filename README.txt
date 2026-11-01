CAMPUSCONNECT (C++14)

FILES
- Student.h / Student.cpp: Student profile, skills, interests and connections.
- CampusConnect.h / CampusConnect.cpp: linked-list student storage, linked-list queue,
  searching, merge sort, and connection-request operations.
- Auth.h / Auth.cpp: create account, login, logout, and credential storage.
- FileHandler.h / FileHandler.cpp: save/load student records and pending requests.
- main.cpp: program entry point, menus, and exception handling.
- students.txt: created automatically to store profiles, skills, interests and connections.
- requests.txt: created automatically to store pending requests.
- accounts.txt: created automatically to store student IDs and demo passwords.

COMPILE (VS Code terminal, from this folder)
g++ -std=c++14 main.cpp Student.cpp CampusConnect.cpp FileHandler.cpp Auth.cpp -o CampusConnect

RUN ON WINDOWS
.\CampusConnect.exe

RUN ON Linux/macOS
./CampusConnect

FIRST RUN
Choose 1 to create an account. Enter a positive student ID, profile details and a password.
Choose 2 to log in. If a profile already exists in students.txt but has no account, you can
create a login for that existing ID. Save and exit to write the data files.

DATA FILES
The files are created automatically when the program saves data. Keep them in the same
working directory when running the program. If files do not exist on the first run, the
program starts with empty data. Do not edit their formats manually.

SECURITY NOTE
This is an educational console project. Passwords in accounts.txt are plain text for
simplicity and are NOT secure for real-world use. A real application must hash passwords.

DSA USED
- Singly linked lists for students, skills, interests and accepted connections.
- Queue (linked-list implementation) for pending requests.
- Linear search by ID, branch and skill.
- Merge sort by student name: O(n log n) time.

EXCEPTION HANDLING
Startup loading, saving and menu operations use try/catch. Numeric input is validated.
Malformed text files and file open/write failures are reported.
