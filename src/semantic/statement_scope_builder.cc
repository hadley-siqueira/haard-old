#include <iostream>
#include "semantic/statement_scope_builder.h"
#include "semantic/compound_statement_scope_builder.h"

using namespace haard;

StatementScopeBuilder::StatementScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void StatementScopeBuilder::build_statement(Statement* stmt) {
    std::cout << "uh oh\n";
    switch (stmt->get_kind()) {
    case STMT_COMPOUND:
        build_compound_statement((CompoundStatement*) stmt);
        break;

    default:
        break;
    }
}

void StatementScopeBuilder::build_compound_statement(CompoundStatement* stmt) {
    CompoundStatementScopeBuilder builder(get_context());

    builder.build_compound_statement(stmt);
}
