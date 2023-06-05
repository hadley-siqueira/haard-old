#include "ast/unop.h"

using namespace haard;

UnOp::UnOp() {
    kind = EXPR_UNKNOWN;
    expression = nullptr;
}

UnOp::UnOp(int kind, Expression* expression) {
    this->kind = kind;
    this->expression = expression;
}

UnOp::UnOp(int kind, Token& token, Expression* expression) {
    this->kind = kind;
    this->expression = expression;
    line = token.get_line();
    column = token.get_column();
}

UnOp::~UnOp() {
    delete expression;
}

Expression* UnOp::get_expression() {
    return expression;
}

void UnOp::set_expression(Expression* expression) {
    this->expression = expression;
}
