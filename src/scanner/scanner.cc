#include <iostream>
#include <fstream>
#include "scanner/scanner.h"
#include "defs.h"

using namespace haard;

Scanner::Scanner() {
    line = 1;
    column = 1;
    idx = 0;
    ws = 0;
}

std::vector<Token> Scanner::read(std::string path) {
    read_to_buffer(path);

    while (has_next()) {
        get_token();
    }

    start_token();
    create_token(TK_EOF);
    return tokens;
}

void Scanner::read_to_buffer(std::string path) {
    char c;
    std::ifstream file;

    file.open(path.c_str());

    while (file.get(c)) {
        buffer += c;
    }

    file.close();
}

bool Scanner::has_next() {
    return idx < buffer.size();
}

bool Scanner::lookahead(char c) {
    return has_next() && buffer[idx] == c;
}

bool Scanner::lookahead(char c, int offset) {
    return has_next() && buffer[idx + offset] == c;
}

bool Scanner::is_binary_digit() {
    return buffer[idx] == '0' || buffer[idx] == '1';
}

bool Scanner::is_octal_digit() {
    return buffer[idx] >= '0' && buffer[idx] <= '7';
}

bool Scanner::is_hex_digit() {
    return buffer[idx] >= '0' && buffer[idx] <= '9' ||
           buffer[idx] >= 'a' && buffer[idx] <= 'f' ||
           buffer[idx] >= 'A' && buffer[idx] <= 'F';
}

bool Scanner::is_alpha(int offset) {
    return buffer[idx + offset] >= 'a' && buffer[idx + offset] <= 'z' ||
           buffer[idx + offset] >= 'A' && buffer[idx + offset] <= 'Z' ||
           buffer[idx + offset] == '_';
}

bool Scanner::is_num(int offset) {
    return buffer[idx + offset] >= '0' && buffer[idx + offset] <= '9';
}

bool Scanner::is_alphanum(int offset) {
    return is_alpha(offset) || is_num(offset);
}

bool Scanner::is_operator() {
    char c = buffer[idx];

    return c == '(' || c == ')' || c == '[' || c == ']'
        || c == '{' || c == '}' || c == '+' || c == '-'
        || c == '*' || c == '/' || c == '%' || c == '!'
        || c == '&' || c == '|' || c == '~' || c ==  '='
        || c == '>' || c == '<' || c == '^' || c == '.'
        || c == '$' || c == ':' || c == '?' || c == '@'
        || c == ',' || c == ';'; 
}

bool Scanner::has_base() {
    bool flag;
    bool flag2;

    if (idx + 2 >= buffer.size()) return false;

    flag = buffer[idx + 1] == 'o' || buffer[idx + 1] == 'b' || buffer[idx + 1] == 'x';
    flag2 = buffer[idx + 1] >= '0' && buffer[idx + 1] <= '7';

    return buffer[idx] == '0' && (flag || flag2);
}

bool Scanner::is_symbol() {
    return lookahead(':') && is_alphanum(1);
}

void Scanner::start_token() {
    lexeme = "";
    token_line = line;
    token_column = column;
}

void Scanner::advance() {
    if (!has_next()) {
        return;
    }

    if (buffer[idx] == '\n') {
        line++;
        column = 1;
        ws = 0;
    } else {
        column++;
    }

    lexeme += buffer[idx];
    idx++;
}

void Scanner::skip_whitespace() {
    while (lookahead(' ')) {
        advance();
    }
}

void Scanner::skip_comment() {
    while (!lookahead('\n')) {
        advance();
    }
}

void Scanner::get_token() {
    if (lookahead('#')) {
        skip_comment();
    } else if (lookahead('\n')) {
        advance();
        count_leading_whitespace();
    } else if (lookahead(' ')) {
        skip_whitespace();
    } else if (lookahead('"')) {
        get_double_quote_string();
    } else if (lookahead('\'')) {
        get_single_quote_string();
    } else if (is_symbol()) {
        get_symbol();
    } else if (is_alpha()) {
        get_keyword_or_identifier();
    } else if (is_operator()) {
        get_operator();
    } else if (is_num()) {
        get_number();
    }
}

void Scanner::get_keyword_or_identifier() {
    int kind;

    start_token();

    while (is_alphanum()) {
        advance();
    }

    kind = TK_ID;

    if (lexeme_to_token_kind_map.count(lexeme) > 0) {
        kind = lexeme_to_token_kind_map.at(lexeme);
    } 

    create_token(kind);
}

void Scanner::get_operator() {
    int kind;
    std::string tmp;

    for (int i = 0; i < 4; ++i) {
        tmp += buffer[idx + i]; 
    }

    while (tmp.size() > 0) {
        if (lexeme_to_token_kind_map.count(tmp) > 0) {
            kind = lexeme_to_token_kind_map.at(tmp);
            break;
        }

        tmp.pop_back();
    }

    if (tmp.size() > 0) {
        start_token();

        for (int i = 0; i < tmp.size(); ++i) {
            advance();
        }

        create_token(kind);
    }

    // handle invalid operator
}

void Scanner::get_double_quote_string() {
    start_token();
    advance();

    while (!lookahead('"')) {
        if (lookahead('\\')) {
            advance();
        }

        advance();
    }

    advance();
    create_token(TK_LITERAL_STRING);
}

void Scanner::get_single_quote_string() {
    int steps = 0;

    start_token();
    advance();

    while (!lookahead('\'')) {
        if (lookahead('\\')) {
            advance();
        }

        advance();
        steps++;
    }

    advance();

    if (steps > 1) {
        create_token(TK_LITERAL_STRING);
    } else {
        create_token(TK_LITERAL_CHAR);
    }
}

void Scanner::get_number() {
    int kind = TK_LITERAL_INTEGER;

    start_token();

    if (has_base()) {
        advance();

        if (lookahead('b')) {
            advance();

            while (is_binary_digit()) {
                advance();
            }
        } else if (lookahead('o')) {
            advance();

            while (is_octal_digit()) {
                advance();
            }
        } else if (lookahead('x')) {
            advance(); 

            while (is_hex_digit()) {
                advance();
            }
        } else {
            while (is_octal_digit()) {
                advance();
            }
        }
    } else {
        while (is_num()) {
            advance();
        }

        if (lookahead('.') && !lookahead('.', 1)) {
            advance();
            kind = TK_LITERAL_DOUBLE;

            while (is_num()) {
                advance();
            }
        }

        if (lookahead('e') || lookahead('E')) {
            kind = TK_LITERAL_DOUBLE;
            advance();

            if (lookahead('-') || lookahead('+')) {
                advance();
            }

            while (is_num()) {
                advance();
            }
        }

        if (lookahead('f') || lookahead('F')) {
            advance();
            kind = TK_LITERAL_FLOAT;
        }
    }

    create_token(kind);
}

void Scanner::get_symbol() {
    start_token();
    advance();

    while (is_alphanum()) {
        advance();
    }

    create_token(TK_LITERAL_SYMBOL);
}

void Scanner::create_token(int kind) {
    Token token;

    token.set_kind(kind);
    token.set_lexeme(lexeme);
    token.set_line(token_line);
    token.set_column(token_column);
    token.set_whitespace(ws);

    tokens.push_back(token);
}

void Scanner::count_leading_whitespace() {
    ws = 0;

    while (lookahead(' ')) {
        advance();
        ws++;
    }
}
