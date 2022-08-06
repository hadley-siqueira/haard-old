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

void Logger::info(std::string path, int line, int column, std::string msg) {
    logs.push_back(new Log(LOG_INFO, line, column, path, msg));
}

void Logger::warn(std::string path, int line, int column, std::string msg) {
    logs.push_back(new Log(LOG_WARNING, line, column, path, msg));
}

void Logger::error(std::string path, int line, int column, std::string msg) {
    logs.push_back(new Log(LOG_ERROR, line, column, path, msg));
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

    std::cout << "main.hd:181:10: ;<red>error:</red> unknown type:\n";
    std::cout << do_message("    @a : int", 181, 10, 2) << '\n';

    std::cout << "\nmain.hd:181:10: error: unexpected end of expression:\n";
    std::cout << do_message("       if obj.get():", 181, 15, 5) << '\n';
    
    std::cout << colorify("main.hd:181:10: ;<red>error:</red> unknown <yellow>type:</yellow> and now is <white>white</white> and this one is <magenta>warning</magenta>\n") << '\n';
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

std::string Logger::do_message(std::string buf, int line, int column, int count) {
    std::stringstream ss;

    ss << "  " << line << " |";
    ss << colorify(RED, buf, column, count) << '\n';
    ss << create_trailing(line, column, count);

    return ss.str();
}
