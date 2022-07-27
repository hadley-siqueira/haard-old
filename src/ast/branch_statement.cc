#include "ast/branch_statement.h"

using namespace haard;

BranchStatement::BranchStatement(int kind) {
    this->kind = kind;
    true_statements = nullptr;
    false_statements = nullptr;
    condition = nullptr;
}

BranchStatement::~BranchStatement() {
    delete true_statements;
    delete false_statements;
    delete condition;
}

Expression* BranchStatement::get_condition() {
    return condition;
}

Statement* BranchStatement::get_true_statements() {
    return true_statements;
}

Statement* BranchStatement::get_false_statements() {
    return false_statements;
}

void BranchStatement::set_condition(Expression* expression) {
    condition = expression;
}

void BranchStatement::set_true_statements(Statement* stmt) {
    true_statements = stmt;
}

void BranchStatement::set_false_statements(Statement* stmt) {
    false_statements = stmt;
}