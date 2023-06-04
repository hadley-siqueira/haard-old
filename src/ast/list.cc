#include <sstream>
#include "ast/list.h"

using namespace haard;

List::List() {
    set_kind(EXPR_LIST);
    expressions = new ExpressionList(EXPR_LIST);
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

std::string List::to_str() {
    return expressions->to_str("[", "]");
}
