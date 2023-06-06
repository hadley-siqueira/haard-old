#include "ast/else.h"

using namespace haard;

Else::Else() {
    set_kind(STMT_ELSE);
    set_statements(nullptr);
}

Else::~Else() {
    delete statements;
}

Statement* Else::get_statements() const {
    return statements;
}

void Else::set_statements(Statement* value) {
    statements = value;
}
