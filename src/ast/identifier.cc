#include <sstream>
#include "ast/identifier.h"
#include "utils/string_pool.h"

using namespace haard;

Identifier::Identifier() {
    kind = EXPR_ID;
    symbol = nullptr;
    lexeme = nullptr;
    overloaded_index = 0;
}

Identifier::Identifier(Token& token) {
    kind = EXPR_ID;
    line = token.get_line();
    column = token.get_column();
    lexeme = token.get_lexeme();
    symbol = nullptr;
    overloaded_index = 0;
}

Identifier::Identifier(std::string name) {
    kind = EXPR_ID;
    line = 0;
    column = 0;
    lexeme = StringPool::get(name);
    symbol = nullptr;
    overloaded_index = 0;
}

int Identifier::get_line() {
    return line;
}

int Identifier::get_column() {
    return column;
}

const char* Identifier::get_lexeme() {
    return lexeme;
}

Symbol* Identifier::get_symbol() {
    return symbol;
}

void Identifier::set_line(int line) {
    this->line = line;
}

void Identifier::set_column(int column) {
    this->column;
}

void Identifier::set_lexeme(const char* lexeme) {
    this->lexeme = lexeme;
}

void Identifier::set_symbol(Symbol* symbol) {
    this->symbol = symbol;
    set_type(symbol->get_type());
}

int Identifier::get_overloaded_index() {
    return overloaded_index;
}

void Identifier::set_overloaded_index(int idx) {
    overloaded_index = idx;
}

std::string Identifier::get_unique_name() {
    return symbol->to_cpp(overloaded_index);
}

Expression* Identifier::clone() {
    Identifier* other = new Identifier();

    other->line = line;
    other->column = column;
    other->lexeme = lexeme;

    return other;
}
