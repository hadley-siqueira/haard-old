#include <sstream>
#include "ast/list.h"

using namespace haard;

List::List() {
    set_kind(EXPR_LIST);
    expressions = new ExpressionList();
}

List::~List() {
    delete expressions;
}

void List::add_expression(Expression* expr) {
    expressions->add_expression(expr);
}

int List::expressions_count() {
    return expressions->expressions_count();
}

Expression* List::get_expression(int idx) {
    return expressions->get_expression(idx);
}

ExpressionList* List::get_expressions() const {
    return expressions;
}

void List::set_expressions(ExpressionList* value) {
    expressions = value;
}
