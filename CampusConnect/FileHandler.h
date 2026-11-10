#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "CampusConnect.h"
#include <string>

class FileHandler {
public:
    static void save(const CampusConnect& app, const string& studentFile = "students.txt",
                     const string& requestFile = "requests.txt");
    static void load(CampusConnect& app, const string& studentFile = "students.txt",
                     const string& requestFile = "requests.txt");
};

#endif
