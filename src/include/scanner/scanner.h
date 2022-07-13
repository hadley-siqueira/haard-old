#ifndef HAARD_SCANNER_H
#define HAARD_SCANNER_H

#include <vector>
#include "token/token.h"

namespace haard {
    class Scanner {
        public:
            Scanner();
            std::vector<Token> read(std::string path);

        private:
            bool has_next();
            bool lookahead(char c);
            bool is_binary_digit();
            bool is_octal_digit();
            bool is_hex_digit();
            bool is_alpha();
            bool is_num();
            bool is_alphanum();
            bool is_operator();
            bool has_base();
            void start_token();
            void advance();
            void skip_whitespace();
            void skip_comment();
            void read_to_buffer(std::string path);
            void get_token();
            void get_keyword_or_identifier();
            void get_operator();
            void get_double_quote_string();
            void get_single_quote_string();
            void get_number();
            void create_token(int kind);
            void count_leading_whitespace();

        private:
            int line;
            int column;
            int token_line;
            int token_column;
            int idx;
            int ws;
            std::vector<Token> tokens;
            std::string buffer;
            std::string lexeme;
    };
}

#endif
