#include "defs.h"
#include "ast/compound_statement.h"

using namespace haard;

CompoundStatement::CompoundStatement() {
    kind = STMT_COMPOUND;
}

CompoundStatement::~CompoundStatement() {
    for (int i = 0; i < statements.size(); ++i) {
        delete statements[i];
    }
}

Statement* CompoundStatement::get_statement(int idx) {
    if (idx < statements.size()) {
        return statements[idx];
    }

    return nullptr;
}

int CompoundStatement::statements_count() {
    return statements.size();
}

void CompoundStatement::add_statement(Statement* stmt) {
    statements.push_back(stmt);
}

Statement* CompoundStatement::clone() {
    CompoundStatement* other = new CompoundStatement();

    for (int i = 0; i < statements.size(); ++i) {
        other->add_statement(statements[i]->clone());
    }

    return other;
}
