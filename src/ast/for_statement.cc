#include "ast/for_statement.h"

using namespace haard;

ForStatement::ForStatement() {
    kind = STMT_FOR;
    condition = nullptr;
    init = new ExpressionList(EXPR_FOR_INIT);
    increment = new ExpressionList(EXPR_FOR_INC);
    statements = nullptr;
    scope = new Scope();
}

ForStatement::~ForStatement() {
    delete init;
    delete condition;
    delete increment;
    delete statements;
    delete scope;
}

ExpressionList* ForStatement::get_initialization() {
    return init;
}

Expression* ForStatement::get_condition() {
    return condition;
}

ExpressionList* ForStatement::get_increment() {
    return increment;
}

CompoundStatement* ForStatement::get_statements() {
    return statements;
}

void ForStatement::add_initialization(Expression* expression) {
    init->add_expression(expression);
}

void ForStatement::set_condition(Expression* expression) {
    this->condition = expression;
}

void ForStatement::add_increment(Expression* expression) {
    increment->add_expression(expression);
}

void ForStatement::set_statements(CompoundStatement* statements) {
    this->statements = statements;
}


Scope* ForStatement::get_scope() {
    return scope;
}

Statement* ForStatement::clone() {
    ForStatement* other = new ForStatement();

    if (condition) {
        other->condition = condition->clone();
    }

    if (init) {
        other->init = (ExpressionList*) init->clone();
    }

    if (statements) {
        other->statements = (CompoundStatement*) statements->clone();
    }

    return other;
}
