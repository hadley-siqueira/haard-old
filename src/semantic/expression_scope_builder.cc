#include <iostream>
#include "semantic/expression_scope_builder.h"
#include "semantic/identifier_scope_builder.h"
#include "semantic/assignment_scope_builder.h"
#include "semantic/plus_scope_builder.h"

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

    case EXPR_LITERAL_BOOL:
        build_literal_bool((Literal*) expr);
        break;

    case EXPR_LITERAL_INTEGER:
        build_literal_integer((Literal*) expr);
        break;

    case EXPR_ASSIGN:
        build_assignment((BinOp*) expr);
        break;

    case EXPR_PLUS:
        build_plus((Plus*) expr);
        break;

    default:
        break;
    }
}

void ExpressionScopeBuilder::build_identifier(Identifier* id) {
    IdentifierScopeBuilder builder(get_context());

    builder.build_identifier(id);
}

void ExpressionScopeBuilder::build_literal_bool(Literal* literal) {
    literal->set_type(new Type(TYPE_BOOL));
}

void ExpressionScopeBuilder::build_literal_char(Literal* literal) {
    literal->set_type(new Type(TYPE_BOOL));
}

void ExpressionScopeBuilder::build_literal_integer(Literal* literal) {
    literal->set_type(new Type(TYPE_INT));
}

void ExpressionScopeBuilder::build_literal_float(Literal* literal) {
    literal->set_type(new Type(TYPE_FLOAT));
}

void ExpressionScopeBuilder::build_literal_double(Literal* literal) {
    literal->set_type(new Type(TYPE_DOUBLE));
}

void ExpressionScopeBuilder::build_literal_symbol(Literal* literal) {
    literal->set_type(new Type(TYPE_SYMBOL));
}

void ExpressionScopeBuilder::build_assignment(BinOp* bin) {
    AssignmentScopeBuilder builder(get_context());

    builder.build_assignment(bin);
}

void ExpressionScopeBuilder::build_plus(Plus* oper) {
    PlusScopeBuilder builder(get_context());

    builder.build_plus(oper);
}
