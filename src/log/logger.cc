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

    std::cout << do_message("    @a : int", 181, 10, 2) << '\n';
    std::cout << buffer;
    return buffer;
}

#define RED 0
#define REDD "\033[31m"
#define NORMAL "\033[0m"

std::string colorify(int color, std::string msg, int column, int count) {
    std::stringstream ss;
    int c_end = column + count;

    for (int i = 0; i < msg.size(); ++i) {
        if (i == column - 1) {
            ss << REDD;
        } 

        ss << msg[i];

        if (i == column + count - 2) {
            ss << NORMAL;
        }
    }

    return ss.str();
}

std::string create_trailing(int line, int column, int count) {
    std::stringstream ss;
    int s1;

    ss << "  " << line << " |";
    s1 = ss.str().size();

    ss.str("");

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

    ss << "    ";
    return colorify(RED, ss.str(), column + s1, count);
}

std::string Logger::do_message(std::string buf, int line, int column, int count) {
    std::stringstream ss;

    ss << "  " << line << " |";
    ss << colorify(RED, buf, column, count) << '\n';
    ss << create_trailing(line, column, count);

    return ss.str();
}
