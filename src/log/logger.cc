#include <iostream>
#include <fstream>
#include <sstream>
#include "log/logger.h"

using namespace haard;

Logger::~Logger() {
    for (int i = 0; i < logs.size(); ++i) {
        delete logs[i];
    }
}

void Logger::add_log(Log* log) {
    logs.push_back(log);
}

std::string Logger::read_file(const char* path, int lbegin, int count) {
    std::string buffer;
    std::ifstream file(path);
    int line = 1;
    char c;

    while (line != lbegin && file.get(c)) {
        if (c == '\n') {
            ++line;
        }
    }

    while (line != lbegin + count && file.get(c)) {
        buffer += c;

        if (c == '\n') {
            ++line;
        }
    }

    std::cout << do_message("    @a", 181, 5, 1);
    std::cout << buffer;
    return buffer;
}

std::string Logger::do_message(std::string buf, int line, int column, int count) {
    std::stringstream ss;
    int s1;

    ss << "  " << line << " |";
    s1 = ss.str().size();

    ss << buf << '\n';
    for (int i = 0; i < s1 - 1; ++i) {
        ss << ' ';
    }

    ss << "|";

    for (int i = 0; i < column - 1; ++i) {
        ss << ' ';
    }

    for (int i = 0; i < count; ++i) {
        ss << '^';
    }

    return ss.str();
}
