#include <sstream>
#include "ast/tuple.h"

using namespace haard;

Tuple::Tuple() {
    set_kind(EXPR_TUPLE);
    expressions = new ExpressionList();
}

Tuple::~Tuple() {
    delete expressions;
}

void Tuple::add_expression(Expression* expr) {
    expressions->add_expression(expr);
}

int Tuple::expressions_count() {
    return expressions->expressions_count();
}

Expression* Tuple::get_expression(int idx) {
    return expressions->get_expression(idx);
}
