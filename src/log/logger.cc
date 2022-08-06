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

void Logger::error(std::string path, int line, int column, std::string msg) {
    std::stringstream ss;

    std::string c = read_line(path.c_str(), line);
    std::string cf = do_message(c, RED, line, column, 1);

    ss << colorify(msg) << '\n';
    ss << cf << '\n';
    
    logs.push_back(new Log(LOG_ERROR, line, column, path, ss.str()));
    print();
    exit(0);
}

void Logger::error(std::string path, Token& token, std::string msg) {
    std::stringstream ss;
    int line = token.get_line();
    int column = token.get_column();
    int count = strlen(token.get_lexeme());

    std::string c = read_line(path.c_str(), token.get_line());
    std::string cf = do_message(c, RED, line, column, count);

    ss << colorify(msg) << '\n';
    ss << cf << '\n';
    
    logs.push_back(new Log(LOG_ERROR, line, column, path, ss.str()));
    print();
    exit(0);
}

std::string Logger::read_line(const char* path, int line) {
    std::ifstream file(path);
    std::string str;

    for (int i = 0; i < line; ++i) {
        getline(file, str);
    }

    return str;
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
