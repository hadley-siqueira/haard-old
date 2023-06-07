#include <sstream>
#include "ast/hash.h"

using namespace haard;

Hash::Hash() {
    set_kind(EXPR_HASH);
    expressions = new ExpressionList();
}

Hash::~Hash() {
    delete expressions;
}

void Hash::add_expression(Expression* expr) {
    expressions->add_expression(expr);
}

int Hash::expressions_count() {
    return expressions->expressions_count();
}

Expression* Hash::get_expression(int idx) {
    return expressions->get_expression(idx);
}

ExpressionList* Hash::get_expressions() const {
    return expressions;
}

void Hash::set_expressions(ExpressionList* value) {
    expressions = value;
}
