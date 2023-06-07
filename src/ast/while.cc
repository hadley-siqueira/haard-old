#include "ast/while.h"

using namespace haard;

While::While() {
    set_kind(STMT_WHILE);
    set_condition(nullptr);
    set_statements(nullptr);
}

While::~While() {
    delete condition;
    delete statements;
}

Expression* While::get_condition() {
    return condition;
}

CompoundStatement* While::get_statements() {
    return statements;
}

void While::set_condition(Expression* expression) {
    this->condition = expression;
}

void While::set_statements(CompoundStatement* statements) {
    this->statements = statements;
}
