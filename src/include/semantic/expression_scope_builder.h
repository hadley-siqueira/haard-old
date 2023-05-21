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

    private:
        void build_identifier(Identifier* id);
        void build_literal_bool(Literal* literal);
        void build_literal_char(Literal* literal);
        void build_literal_integer(Literal* literal);
        void build_literal_float(Literal* literal);
        void build_literal_double(Literal* literal);
        void build_literal_symbol(Literal* literal);

        void build_assignment(BinOp* bin);
        void build_plus(BinOp* oper);

    };
}

#endif
