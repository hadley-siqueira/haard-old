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

bool Scanner::is_alpha() {
    return buffer[idx] >= 'a' && buffer[idx] <= 'z' ||
           buffer[idx] >= 'A' && buffer[idx] <= 'Z' ||
           buffer[idx] == '_';
}

bool Scanner::is_num() {
    return buffer[idx] >= '0' && buffer[idx] <= '9';
}

bool Scanner::is_alphanum() {
    return is_alpha() || is_num();
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
    } else if (is_alpha()) {
        get_keyword_or_identifier();
    } else if (is_operator()) {
        get_operator();
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
    advance();
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
