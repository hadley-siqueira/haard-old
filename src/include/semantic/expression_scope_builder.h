#ifndef HAARD_EXPRESSION_SCOPE_BUILDER_H
#define HAARD_EXPRESSION_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "semantic/scope_builder.h"

namespace haard {
    class ExpressionScopeBuilder : public ScopeBuilder {
    public:
        ExpressionScopeBuilder(ScopeBuilderContext* context);

    public:
        void build_expression(Expression* expr);
        void build_identifier(Identifier* id);
        void build_assignment(BinOp* bin);

    };
}

#endif
