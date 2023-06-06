#include "ast/jump_statement.h"

using namespace haard;

JumpStatement::JumpStatement(int kind) {
    set_kind(kind);
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
