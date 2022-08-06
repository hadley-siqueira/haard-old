#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <map>
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

void Logger::error(std::string path, int line, int column, std::string msg) {
    std::stringstream ss;


    std::string c = read_file(path.c_str(), line, 1);
    std::string cf = do_message(c, RED, line, column, 1);

    ss << colorify(msg) << '\n';
    ss << c << '\n';
    ss << cf << '\n';
    
    logs.push_back(new Log(LOG_ERROR, line, column, path, ss.str()));
    print();
    exit(0);
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

    return buffer;
}


std::string colorify(std::string msg) {
    std::stringstream ss;
    std::vector<std::string> pieces;
    std::stack<std::string> color_stack;

    std::map<std::string, std::string> color_map = {
        {"<black>", BLACK},
        {"<red>", RED},
        {"<green>", GREEN},
        {"<yellow>", YELLOW},
        {"<blue>", BLUE},
        {"<magenta>", MAGENTA},
        {"<cyan>", CYAN},
        {"<white>", WHITE},
    };

    for (int i = 0; i < msg.size(); ++i) {
        if (msg[i] == '<') {
            pieces.push_back(ss.str());
            ss.str("");

            while (msg[i] != '>') {
                ss << msg[i];
                ++i;
            }

            ss << msg[i];
            pieces.push_back(ss.str());
            ss.str("");
        } else {
            ss << msg[i];
        }
    }

    pieces.push_back(ss.str());
    ss.str("");
    color_stack.push(NORMAL);

    for (int i = 0; i < pieces.size(); ++i) {
        std::cout << pieces[i] << '\n';

        if (color_map.count(pieces[i]) > 0) {
            ss << color_map[pieces[i]];
            color_stack.push(color_map[pieces[i]]);
        } else if (pieces[i] == "<yellow>") {
            ss << YELLOW;
            color_stack.push(YELLOW);
        } else if (pieces[i].size() >= 2 && pieces[i][0] == '<' && pieces[i][1] == '/') {
            color_stack.pop();
            ss << color_stack.top();
        } else {
            ss << pieces[i];
        }
    }
    
    return ss.str();
}

std::string colorify(std::string color, std::string msg, int column, int count) {
    std::stringstream ss;
    int c_end = column + count;

    for (int i = 0; i < msg.size(); ++i) {
        if (i == column - 1) {
            ss << color;
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

std::string Logger::do_message(std::string buf, std::string color, int line, int column, int count) {
    std::stringstream ss;

    ss << "  " << line << " |";
    ss << colorify(color, buf, column, count) << '\n';
    ss << create_trailing(line, column, count);

    return ss.str();
}
