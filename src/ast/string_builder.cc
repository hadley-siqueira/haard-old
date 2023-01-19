#include "ast/string_builder.h"

using namespace haard;

StringBuilder::StringBuilder() {
    variable = new Variable();
    set_kind(EXPR_STRING_BUILDER);
}

StringBuilder::~StringBuilder() {
    /*for (int i = 0; i < expressions_count(); ++i) {
        delete expressions[i];
    }*/

    for (int i = 0; i < calls_count(); ++i) {
        delete calls[i];
    }
}

void StringBuilder::add_expression(Expression* expr) {
    expressions.push_back(expr);
}

int StringBuilder::expressions_count() {
    return expressions.size();
}

Expression* StringBuilder::get_expression(int i) {
    if (i < expressions_count()) {
        return expressions[i];
    }

    return nullptr;
}

void StringBuilder::add_call(Expression* expr) {
    calls.push_back(expr);
}

int StringBuilder::calls_count() {
    return calls.size();
}

Expression* StringBuilder::get_call(int i) {
    if (i < calls_count()) {
        return calls[i];
    }

    return nullptr;
}

Variable* StringBuilder::get_variable() const {
    return variable;
}

void StringBuilder::set_variable(Variable* value) {
    variable = value;
}
