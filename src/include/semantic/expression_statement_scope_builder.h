#ifndef HAARD_EXPRESSION_STATEMENT_SCOPE_BUILDER_H
#define HAARD_EXPRESSION_STATEMENT_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "semantic/scope_builder.h"

namespace haard {
    class ExpressionStatementScopeBuilder : public ScopeBuilder {
    public:
        ExpressionStatementScopeBuilder(ScopeBuilderContext* context);

    public:
        void build_expression_statement(ExpressionStatement* stmt);

    };
}

#endif
