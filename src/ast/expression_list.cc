#include "ast/expression_list.h"

using namespace haard;

ExpressionList::ExpressionList(int kind) {
    this->kind = kind;
}

ExpressionList::ExpressionList(int kind, Expression* expression) {
    this->kind = kind;
    add_expression(expression);
}

ExpressionList::ExpressionList(int kind, std::vector<Expression*> exprs) {
    this->kind = kind;

    for (int i = 0; i < exprs.size(); ++i) {
        add_expression(exprs[i]);
    }
}

ExpressionList::~ExpressionList() {
    for (int i = 0; i < expressions.size(); ++i) {
        delete expressions[i];
    }
}

Expression* ExpressionList::get_expression(int idx) {
    if (idx < expressions.size()) {
        return expressions[idx];
    }

    return nullptr;
}

void ExpressionList::add_expression(Expression* expression) {
    expressions.push_back(expression);
}

int ExpressionList::expressions_count() {
    return expressions.size();
}

Expression* ExpressionList::clone() {
    ExpressionList* other = new ExpressionList(kind);

    for (int i = 0; i < expressions.size(); ++i) {
        other->add_expression(expressions[i]->clone());
    }

    return other;
}
