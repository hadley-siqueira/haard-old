#include "ast/array_list_type.h"

using namespace haard;

ArrayListType::ArrayListType() {

}

ArrayListType::ArrayListType(int kind, Token& token, Type* subtype, Expression* expr) {
    this->kind = kind;
    this->subtype = subtype;
    this->expression = expr;

    line = token.get_line();
    column = token.get_column();
}

ArrayListType::~ArrayListType() {
    delete expression;
}

Expression* ArrayListType::get_expression() {
    return expression;
}

Type* ArrayListType::get_subtype() {
    return subtype;
}

Type* ArrayListType::clone() {
    ArrayListType* other = new ArrayListType();

    other->kind = kind;
    other->line = line;
    other->column = column;

    if (expression) {
        other->expression = expression->clone();
    }

    if (subtype) {
        other->subtype = subtype->clone();
    }

    return other;
}
