#include <iostream>
#include "semantic/expression_statement_scope_builder.h"
#include "semantic/expression_scope_builder.h"

using namespace haard;

ExpressionStatementScopeBuilder::ExpressionStatementScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void ExpressionStatementScopeBuilder::build_expression_statement(ExpressionStatement* stmt) {
    ExpressionScopeBuilder builder(get_context());

    builder.build_expression(stmt->get_expression());
}
