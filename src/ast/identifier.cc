#include <sstream>
#include "ast/identifier.h"

using namespace haard;

Identifier::Identifier() {
    kind = EXPR_ID;
    symbol = nullptr;
    overloaded_index = 0;
    template_header = nullptr;
}

Identifier::Identifier(Token& token) {
    kind = EXPR_ID;
    line = token.get_line();
    column = token.get_column();
    name = token.get_lexeme();
    symbol = nullptr;
    overloaded_index = 0;
    template_header = nullptr;
}

Identifier::Identifier(Token& scope, Token& name) {
    kind = EXPR_ID;
    line = name.get_line();
    column = name.get_column();
    this->name = name.get_lexeme();
    this->alias = scope.get_lexeme();
    symbol = nullptr;
    overloaded_index = 0;
    template_header = nullptr;
}

Identifier::Identifier(std::string name) {
    kind = EXPR_ID;
    line = 0;
    column = 0;
    this->name = name;
    symbol = nullptr;
    overloaded_index = 0;
    template_header = nullptr;
}

Identifier::~Identifier() {
    delete template_header;
}

int Identifier::get_line() {
    return line;
}

int Identifier::get_column() {
    return column;
}

std::string Identifier::get_name() {
    return name;
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

void Identifier::set_name(std::string lexeme) {
    this->name = lexeme;
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
    return symbol->get_qualified_name(overloaded_index);
}

Identifier* Identifier::clone() {
    Identifier* other = new Identifier();

    other->line = line;
    other->column = column;
    other->name = name;

    return other;
}

bool Identifier::is_local_variable() {
    if (symbol == nullptr) {
        return false;
    }

    int kind = symbol->get_kind();
    return kind == SYM_VARIABLE || kind == SYM_PARAMETER;
}

bool Identifier::is_class_variable() {
    if (symbol == nullptr) {
        return false;
    }

    int kind = symbol->get_kind();
    return kind == SYM_CLASS_VARIABLE;
}

bool Identifier::has_scope() {
    return alias.size() > 0;
}

bool Identifier::has_global_scope() {
    return alias == "::";
}

std::string Identifier::get_alias() const {
    return alias;
}

void Identifier::set_alias(const std::string &value) {
    alias = value;
}

TemplateHeader* Identifier::get_template_header() const {
    return template_header;
}

void Identifier::set_template_header(TemplateHeader *value) {
    template_header = value;
}

bool Identifier::has_template() {
    return template_header != nullptr;
}

void* Identifier::get_descriptor() {
    return symbol->get_descriptor(overloaded_index);
}

std::string Identifier::to_str() {
    std::stringstream ss;

    if (has_scope()) {
        ss << alias;

        if (!has_global_scope()) {
            ss << "::";
        }
    }

    ss << name;

    return ss.str();
}
