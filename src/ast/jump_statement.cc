#include "ast/jump_statement.h"

using namespace haard;

JumpStatement::JumpStatement(int kind) {
    this->kind = kind;
    expression = nullptr;
}

JumpStatement::~JumpStatement() {
    delete expression;
}

Expression* JumpStatement::get_expression() {
    return expression;
}

void JumpStatement::set_expression(Expression* expression) {
    this->expression = expression;
}

Statement* JumpStatement::clone() {
    JumpStatement* other = new JumpStatement(kind);

    if (expression) {
        other->expression = expression->clone();
    }

    return other;
}
