#ifndef HAARD_STATEMENT_SCOPE_BUILDER_H
#define HAARD_STATEMENT_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "semantic/scope_builder.h"

namespace haard {
    class StatementScopeBuilder : public ScopeBuilder {
    public:
        StatementScopeBuilder(ScopeBuilderContext* context);

    public:
        void build_statement(Statement* stmt);
        void build_compound_statement(CompoundStatement* stmt);
        void build_expression_statement(ExpressionStatement* stmt);
    };
}

#endif
