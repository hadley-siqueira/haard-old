#include "ast/while_statement.h"

using namespace haard;

WhileStatement::WhileStatement() {
    kind = STMT_WHILE;
    condition = nullptr;
    statements = nullptr;
    scope = new Scope();
}

WhileStatement::~WhileStatement() {
    delete condition;
    delete statements;
    delete scope;
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

Scope* WhileStatement::get_scope() {
    return scope;
}
