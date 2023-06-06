#include "ast/if.h"

using namespace haard;

If::If() {
    set_kind(STMT_IF);
    set_true_statements(nullptr);
    set_false_statements(nullptr);
}

If::~If() {
    delete true_statements;
    delete false_statements;
}

Expression* If::get_condition() const {
    return condition;
}

void If::set_condition(Expression* value) {
    condition = value;
}

Statement* If::get_true_statements() const {
    return true_statements;
}

void If::set_true_statements(Statement* value) {
    true_statements = value;
}

Statement* If::get_false_statements() const {
    return false_statements;
}

void If::set_false_statements(Statement* value) {
    false_statements = value;
}
