#include <iostream>
#include "semantic/expression_scope_builder.h"
#include "semantic/identifier_scope_builder.h"
#include "semantic/assignment_scope_builder.h"

using namespace haard;

ExpressionScopeBuilder::ExpressionScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void ExpressionScopeBuilder::build_expression(Expression* expr) {
    if (expr == nullptr) {
        return;
    }

    int kind = expr->get_kind();

    switch (kind) {
    case EXPR_ID:
        build_identifier((Identifier*) expr);
        break;

    case EXPR_ASSIGN:
        build_assignment((BinOp*) expr);
        break;

    default:
        break;
    }
}

void ExpressionScopeBuilder::build_identifier(Identifier* id) {
    IdentifierScopeBuilder builder(get_context());

    builder.build_identifier(id);
}

void ExpressionScopeBuilder::build_assignment(BinOp* bin) {
    AssignmentScopeBuilder builder(get_context());

    builder.build_assignment(bin);
}
