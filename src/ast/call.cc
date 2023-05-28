#include <sstream>
#include "ast/call.h"

using namespace haard;


Call::Call(Expression* object, ExpressionList* arguments) {
    this->kind = EXPR_CALL;
    this->object = object;
    this->arguments = arguments;
}

Call::Call(Token& token, Expression* object, ExpressionList* arguments) {
    this->kind = EXPR_CALL;
    this->object = object;
    this->arguments = arguments;
    this->line = token.get_line();
    this->column = token.get_column();
}

Call::~Call() {
    delete object;
    delete arguments;
}

std::string Call::to_str() {
    std::stringstream ss;

    ss << object->to_str();
    ss << "(";
    ss << arguments->to_str();
    ss << ")";

    return ss.str();
}

int Call::get_line() const {
    return line;
}

void Call::set_line(int value) {
    line = value;
}

int Call::get_column() const {
    return column;
}

void Call::set_column(int value) {
    column = value;
}

Expression* Call::get_object() const {
    return object;
}

void Call::set_object(Expression* value) {
    object = value;
}

ExpressionList* Call::get_arguments() const {
    return arguments;
}

void Call::set_arguments(ExpressionList* value) {
    arguments = value;
}
