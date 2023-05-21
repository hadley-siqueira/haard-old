#include "semantic/function_scope_builder.h"
#include "semantic/statement_scope_builder.h"

using namespace haard;

FunctionScopeBuilder::FunctionScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void FunctionScopeBuilder::build_function(Function* function) {
    StatementScopeBuilder stmt_builder(get_context());

    set_function(function);
    context->set_local_var_counter(0);

    if (function->is_template()) {
        return;
    }

    stmt_builder.build_compound_statement(function->get_statements());
}
