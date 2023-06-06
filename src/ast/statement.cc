#include <iostream>
#include "defs.h"
#include "ast/statement.h"
#include "scope/scope.h"

using namespace haard;

Statement::Statement() {
    scope = new Scope();
}

Statement::~Statement() {
    delete scope;
}

Scope* Statement::get_scope() const {
    return scope;
}

void Statement::set_scope(Scope* value) {
    scope = value;
}

