#include <sstream>
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

std::string ExpressionList::to_str() {
    int i;
    std::stringstream ss;

    if (expressions_count() > 0) {
        for (i = 0; i < expressions_count() - 1; ++i) {
            ss << expressions[i]->to_str() << ", ";
        }

        ss << expressions[i]->to_str();
    }

    return ss.str();
}

std::string ExpressionList::to_str(std::string begin, std::string end) {
    std::stringstream ss;

    ss << begin << to_str() << end;
    return ss.str();
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
