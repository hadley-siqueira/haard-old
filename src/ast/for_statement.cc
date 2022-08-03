#include "ast/for_statement.h"

using namespace haard;

ForStatement::ForStatement() {
    kind = STMT_FOR;
    condition = nullptr;
    init = nullptr;
    increment = nullptr;
    statements = nullptr;
}

ForStatement::~ForStatement() {
    delete init;
    delete condition;
    delete increment;
    delete statements;
}

Expression* ForStatement::get_initialization() {
    return init;
}

Expression* ForStatement::get_condition() {
    return condition;
}

Expression* ForStatement::get_increment() {
    return increment;
}

CompoundStatement* ForStatement::get_statements() {
    return statements;
}

void ForStatement::set_initialization(Expression* expression) {
    init = expression;
}

void ForStatement::set_condition(Expression* expression) {
    this->condition = expression;
}

void ForStatement::set_increment(Expression* expression) {
    increment = expression;
}

void ForStatement::set_statements(CompoundStatement* statements) {
    this->statements = statements;
}
