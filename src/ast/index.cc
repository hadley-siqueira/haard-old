#include <sstream>
#include "ast/index.h"

using namespace haard;

Index::Index(Expression* object, Expression* index) {
    this->kind = EXPR_INDEX;
    this->object = object;
    this->index = index;
}

Index::Index(Token& token, Expression* object, Expression* index) {
    this->kind = EXPR_INDEX;
    this->object = object;
    this->index = index;
    this->line = token.get_line();
    this->column = token.get_column();
}

Index::~Index() {
    delete object;
    delete index;
}

Expression* Index::get_object() const {
    return object;
}

void Index::set_object(Expression* value) {
    object = value;
}

Expression* Index::get_index() const {
    return index;
}

void Index::set_index(Expression* value) {
    index = value;
}

std::string Index::to_str() {
    std::stringstream ss;

    ss << object->to_str();
    ss << "[";
    ss << index->to_str();
    ss << "]";

    return ss.str();
}
