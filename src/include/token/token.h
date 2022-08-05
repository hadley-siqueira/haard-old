#ifndef HAARD_TOKEN_H
#define HAARD_TOKEN_H

#include <string>

namespace haard {
    class Token {
        public:
            int get_kind();
            int get_line();
            int get_column();
            int get_whitespace();
            std::string get_lexeme();

            void set_kind(int kind);
            void set_line(int line);
            void set_column(int column);
            void set_whitespace(int whitespace);
            void set_lexeme(std::string lexeme);

            std::string to_str();

        private:
            int kind;
            int line;
            int column;
            int whitespace;
            std::string lexeme;
            const char* lexeme_;
    };
}

#endif
