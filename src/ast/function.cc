#include "ast/function.h"

using namespace haard;

Function::Function() {
    return_type = nullptr;
}

Function::~Function() {
    delete return_type;
    
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

std::string Function::get_name() {
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

void Function::set_line(int line) {
    this->line = line;
}

void Function::set_column(int column) {
    this->column = column;
}

void Function::set_name(std::string name) {
    this->name = name;
}

void Function::set_return_type(Type* type) {
    return_type = type;
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
