#include "semantic/statement_scope_builder.h"

using namespace haard;

void StatementScopeBuilder::build_compound_statement(CompoundStatement* stmt) {
    for (int i = 0; i < stmt->statements_count(); ++i) {
        StatementScopeBuilder builder;
        //builder.build_statement(stmt->get_statement(i));
    }
}
