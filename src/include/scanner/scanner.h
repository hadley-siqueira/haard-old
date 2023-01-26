#ifndef HAARD_SCANNER_H
#define HAARD_SCANNER_H

#include <vector>
#include "token/token.h"

namespace haard {
    class Scanner {
        public:
            Scanner();
            std::vector<Token> read(std::string path);
            std::vector<Token> read_from_string(std::string str);

        private:
            bool has_next();
            bool lookahead(char c);
            bool lookahead(char c, int offset);
            bool is_binary_digit();
            bool is_octal_digit();
            bool is_hex_digit();
            bool is_alpha(int offset=0);
            bool is_num(int offset=0);
            bool is_alphanum(int offset=0);
            bool is_operator();
            bool has_base();
            bool is_symbol();
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
            void get_symbol();
            void create_token(int kind);
            void count_leading_whitespace();

        private:
            int line;
            int column;
            int token_line;
            int token_column;
            int token_begin;
            int token_end;
            int idx;
            int ws;
            std::vector<Token> tokens;
            std::string buffer;
            std::string lexeme;
            bool template_flag;
            int template_counter;
    };
}

#endif
