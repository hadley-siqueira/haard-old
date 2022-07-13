#include <iostream>
#include <fstream>
#include "scanner/scanner.h"

using namespace haard;

Scanner::Scanner() {
    line = 1;
    column = 1;
    idx = 0;
    ws = 0;
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

void Scanner::reset_lexeme() {
    lexeme = "";
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
