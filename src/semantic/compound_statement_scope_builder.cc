#include <iostream>
#include "semantic/compound_statement_scope_builder.h"
#include "semantic/statement_scope_builder.h"

using namespace haard;

CompoundStatementScopeBuilder::CompoundStatementScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void CompoundStatementScopeBuilder::build_compound_statement(CompoundStatement* stmt) {
    enter_scope(stmt->get_scope());

    for (int i = 0; i < stmt->statements_count(); ++i) {
        StatementScopeBuilder builder(get_context());

        builder.build_statement(stmt->get_statement(i));
    }

    leave_scope();
}
