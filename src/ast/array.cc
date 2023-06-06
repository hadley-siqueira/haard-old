#include <sstream>
#include "ast/array.h"

using namespace haard;

Array::Array() {
    set_kind(EXPR_ARRAY);
    expressions = new ExpressionList();
}

Array::~Array() {
    delete expressions;
}

void Array::add_expression(Expression* expr) {
    expressions->add_expression(expr);
}

int Array::expressions_count() {
    return expressions->expressions_count();
}

Expression* Array::get_expression(int idx) {
    return expressions->get_expression(idx);
}

ExpressionList* Array::get_expressions() const {
    return expressions;
}

void Array::set_expressions(ExpressionList* value) {
    expressions = value;
}
