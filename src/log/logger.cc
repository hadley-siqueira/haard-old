#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <map>
#include <cstring>
#include "defs.h"
#include "log/logger.h"

using namespace haard;

std::string colorify(std::string msg);

Logger::~Logger() {
    for (int i = 0; i < logs.size(); ++i) {
        delete logs[i];
    }
}

void Logger::print() {
    for (int i = 0; i < logs.size(); ++i) {
        std::cout << logs[i]->to_str();
    }
}

void Logger::info(std::string path, int line, int column, std::string msg) {
    logs.push_back(new Log(LOG_INFO, line, column, path, msg));
}

void Logger::warn(std::string path, int line, int column, std::string msg) {
    logs.push_back(new Log(LOG_WARNING, line, column, path, msg));
}

void Logger::error(Log* log) {
    logs.push_back(log);
    print();
    exit(0);
}


