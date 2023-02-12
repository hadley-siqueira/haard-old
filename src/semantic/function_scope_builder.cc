#include "semantic/function_scope_builder.h"
#include "semantic/statement_scope_builder.h"

using namespace haard;

void FunctionScopeBuilder::build_function(Function* function) {
    StatementScopeBuilder stmt_builder;

    if (function->is_template()) {
        return;
    }

    stmt_builder.build_compound_statement(function->get_statements());
}
