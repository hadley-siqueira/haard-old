#include <sstream>
#include <vector>
#include <stack>
#include <map>
#include <fstream>
#include "log/utils.h"

namespace haard {
    std::string read_line(std::string path, int line) {
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

        ss << "<red>";

        for (int i = 0; i < count; ++i) {
            ss << '^';
        }

        ss << "</red>";
        ss << "    ";
        return ss.str();
    }

    std::string foobar(std::string path, int line, int column, int count) {
        std::stringstream ss;

        if (count < 1) count = 1;

        ss << "  > " << path << ':' << line << ':' << column << '\n';
        ss << "  " << line << " |";
        ss << read_line(path, line) << "\n";
        ss << create_trailing(line, column, count);

        return ss.str();
    }
}
