#include <iostream>
#include "semantic/statement_scope_builder.h"
#include "semantic/compound_statement_scope_builder.h"
#include "semantic/expression_statement_scope_builder.h"

using namespace haard;

StatementScopeBuilder::StatementScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void StatementScopeBuilder::build_statement(Statement* stmt) {
    switch (stmt->get_kind()) {
    case STMT_COMPOUND:
        build_compound_statement((CompoundStatement*) stmt);
        break;

    case STMT_EXPRESSION:
        build_expression_statement((ExpressionStatement*) stmt);
        break;

    default:
        break;
    }
}

void StatementScopeBuilder::build_compound_statement(CompoundStatement* stmt) {
    CompoundStatementScopeBuilder builder(get_context());

    builder.build_compound_statement(stmt);
}

void StatementScopeBuilder::build_expression_statement(ExpressionStatement* stmt) {
    ExpressionStatementScopeBuilder builder(get_context());

    builder.build_expression_statement(stmt);
}
