#include "ast/array_list_type.h"

using namespace haard;

ArrayListType::ArrayListType(int kind, Token& token, Type* subtype, Expression* expr) {
    this->kind = kind;
    this->subtype = subtype;
    this->expression = expr;

    line = token.get_line();
    column = token.get_column();
}

ArrayListType::~ArrayListType() {
    delete expression;
    delete subtype;
}

Expression* ArrayListType::get_expression() {
    return expression;
}

Type* ArrayListType::get_subtype() {
    return subtype;
}
