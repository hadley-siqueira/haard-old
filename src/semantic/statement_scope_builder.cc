#include "semantic/statement_scope_builder.h"

using namespace haard;

void StatementScopeBuilder::build_compound_statement(CompoundStatement* stmt) {
    /*enter_scope(stmt->get_scope());

    for (int i = 0; i < stmt->statements_count(); ++i) {
        StatementScopeBuilder builder;

        builder.copy_state(this);
        //builder.build_statement(stmt->get_statement(i));
    }

    leave_scope();*/
}
