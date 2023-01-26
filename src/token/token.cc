#include <sstream>
#include "token/token.h"
#include "defs.h"

using namespace haard;

int Token::get_kind() {
    return kind;
}

int Token::get_line() {
    return line;
}

int Token::get_column() {
    return column;
}

int Token::get_whitespace() {
    return whitespace;
}

std::string Token::get_lexeme() {
    return lexeme;
}

void Token::set_kind(int kind) {
    this->kind = kind;
}

void Token::set_line(int line) {
    this->line = line;
}

void Token::set_column(int column) {
    this->column = column;
}

void Token::set_whitespace(int whitespace) {
    this->whitespace = whitespace;
}

void Token::set_lexeme(std::string lexeme) {
    this->lexeme = lexeme;
}

std::string Token::to_str() {
    std::stringstream ss;

    ss << "(";
    ss << token_kind_to_str_map.at(kind) << ", ";
    ss << "'" << lexeme << "', l=";
    ss << line << ", c=";
    ss << column << ", b=";
    ss << begin << ", e=";
    ss << end << ", ws=";
    ss << whitespace << ")";

    return ss.str();
}

int Token::get_begin() const {
    return begin;
}

void Token::set_begin(int value) {
    begin = value;
}

int Token::get_end() const {
    return end;
}

void Token::set_end(int value) {
    end = value;
}
