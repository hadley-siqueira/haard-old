#include "ast/expression_statement.h"

using namespace haard;

ExpressionStatement::ExpressionStatement(Expression* expression) {
    set_kind(STMT_EXPRESSION);
    this->expression = expression;
}

ExpressionStatement::~ExpressionStatement() {
    delete expression;
}

Expression* ExpressionStatement::get_expression() {
    return expression;
}

void ExpressionStatement::set_expression(Expression* expression) {
    this->expression = expression;
}
