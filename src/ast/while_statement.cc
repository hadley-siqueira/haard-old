#include "ast/while_statement.h"

using namespace haard;

WhileStatement::WhileStatement() {
    set_kind(STMT_WHILE);
    set_condition(nullptr);
    set_statements(nullptr);
}

WhileStatement::~WhileStatement() {
    delete condition;
    delete statements;
}

Expression* WhileStatement::get_condition() {
    return condition;
}

CompoundStatement* WhileStatement::get_statements() {
    return statements;
}

void WhileStatement::set_condition(Expression* expression) {
    this->condition = expression;
}

void WhileStatement::set_statements(CompoundStatement* statements) {
    this->statements = statements;
}
