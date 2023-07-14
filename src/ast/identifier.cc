#include <sstream>
#include "ast/identifier.h"

using namespace haard;

Identifier::Identifier() {
    set_kind(EXPR_ID);
    set_symbol_descriptor(nullptr);
    set_symbol_index(0);
    template_header = nullptr;
}

Identifier::Identifier(Token& token) {
    set_kind(EXPR_ID);
    set_line(token.get_line());
    set_column(token.get_column());
    set_name(token.get_lexeme());
    set_symbol_descriptor(nullptr);
    set_symbol_index(0);
    template_header = nullptr;
}

Identifier::Identifier(Token& scope, Token& name) {
    set_kind(EXPR_ID);
    set_line(name.get_line());
    set_column(name.get_column());
    set_name(name.get_lexeme());
    set_symbol_descriptor(nullptr);
    set_symbol_index(0);
    this->alias = scope.get_lexeme();
    template_header = nullptr;
}

Identifier::Identifier(std::string name) {
    set_kind(EXPR_ID);
    set_line(0);
    set_column(0);
    set_name(name);
    set_symbol_index(0);
    set_symbol_descriptor(nullptr);
    template_header = nullptr;
}

Identifier::~Identifier() {
    delete template_header;
}

std::string& Identifier::get_name() {
    return name;
}

SymbolDescriptor* Identifier::get_symbol_descriptor() {
    return symbol_descriptor;
}

void Identifier::set_name(std::string lexeme) {
    this->name = lexeme;
}

void Identifier::set_symbol_descriptor(SymbolDescriptor* value) {
    this->symbol_descriptor = value;

    if (symbol_descriptor) {
        set_type(symbol_descriptor->get_type());
    }
}

bool Identifier::is_local_variable() {
    if (symbol_descriptor == nullptr) {
        return false;
    }

    int kind = symbol_descriptor->get_kind();
    return kind == SYM_VARIABLE || kind == SYM_PARAMETER;
}

bool Identifier::is_class_variable() {
    if (symbol_descriptor == nullptr) {
        return false;
    }

    int kind = symbol_descriptor->get_kind();
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

std::string Identifier::to_str() {
    std::stringstream ss;

    if (has_scope()) {
        ss << alias;

        if (!has_global_scope()) {
            ss << "::";
        }
    }

    ss << name;

    if (template_header) {
        ss << template_header->to_str();
    }

    return ss.str();
}

int Identifier::get_symbol_index() const
{
    return symbol_index;
}

void Identifier::set_symbol_index(int value)
{
    symbol_index = value;
}
