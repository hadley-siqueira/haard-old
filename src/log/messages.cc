#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include <cstring>
#include "log/messages.h"

using namespace haard;

std::string read_line(const char* path, int line) {
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

std::string do_message(std::string buf, std::string color, int line, int column, int count) {
    std::stringstream ss;

    if (count < 1) count = 1;

    ss << "  " << line << " |";
    ss << colorify(color, buf, column, count) << '\n';
    ss << create_trailing(line, column, count);

    return ss.str();
}

Log* haard::error_message_unexpected_token(std::string path, Token& token) {
    std::stringstream ss;
    int line = token.get_line();
    int column = token.get_column();
    int kind = token.get_kind();
    int count = strlen(token.get_lexeme());

    ss << "unexpected token '<white>";
    ss << token_kind_to_str_map.at(kind);
    ss << "</white>'";

    std::string c = read_line(path.c_str(), line);
    std::string cf = do_message(c, RED, line, column, count);
    std::string msg = ss.str();
    ss.str("");

    ss << colorify(msg) << '\n';
    ss << cf << '\n';

    return new Log(LOG_ERROR, line, column, path, ss.str());
}

Log* haard::error_message_expected_token(std::string path, int kind, Token& token) {
    std::stringstream ss;
    int line = token.get_line();
    int column = token.get_column();
    int tkind = token.get_kind();
    int count = strlen(token.get_lexeme());

    ss << "expected token '<white>";
    ss << token_kind_to_str_map.at(kind);
    ss << "</white>' but got a <white>";
    ss << token_kind_to_str_map.at(token.get_kind());
    ss << "</white> instead";

    std::string c = read_line(path.c_str(), line);
    std::string cf = do_message(c, RED, line, column, count);
    std::string msg = ss.str();
    ss.str("");

    ss << colorify(msg) << '\n';
    ss << cf << '\n';

    return new Log(LOG_ERROR, line, column, path, ss.str());
}

Log* haard::error_message_no_return_type(std::string path, Token& token) {
    std::stringstream ss;
    int line = token.get_line();
    int column = token.get_column();
    int tkind = token.get_kind();
    int count = strlen(token.get_lexeme());

    ss << "expected a return type";

    std::string c = read_line(path.c_str(), line);
    std::string cf = do_message(c, RED, line, column, count);
    std::string msg = ss.str();
    ss.str("");

    ss << colorify(msg) << '\n';
    ss << cf << '\n';

    return new Log(LOG_ERROR, line, column, path, ss.str());
}

