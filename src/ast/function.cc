#include "ast/function.h"
#include "scope/scope.h"

using namespace haard;

Function::Function() {
    return_type = nullptr;
    statements = nullptr;
    scope = new Scope();
}

Function::~Function() {
    delete return_type;
    delete statements;
    delete scope;
    
    for (int i = 0; i < parameters.size(); ++i) {
        delete parameters[i];
    }
}

int Function::get_line() {
    return line;
}

int Function::get_column() {
    return column;
}

const char* Function::get_name() {
    return name;
}

Variable* Function::get_parameter(int idx) {
    if (idx < parameters.size()) {
        return parameters[idx];
    }

    return nullptr;
}

Type* Function::get_return_type() {
    return return_type;
}

CompoundStatement* Function::get_statements() {
    return statements;
}

Scope* Function::get_scope() {
    return scope;
}

void Function::set_line(int line) {
    this->line = line;
}

void Function::set_column(int column) {
    this->column = column;
}

void Function::set_name(const char* name) {
    this->name = name;
}

void Function::set_return_type(Type* type) {
    return_type = type;
}

void Function::set_statements(CompoundStatement* statements) {
    this->statements = statements;
}

int Function::parameters_count() {
    return parameters.size();
}

void Function::add_parameter(Variable* param) {
    parameters.push_back(param);
}
            
void Function::set_from_token(Token& token) {
    line = token.get_line();
    column = token.get_column();
    name = token.get_lexeme();
}
