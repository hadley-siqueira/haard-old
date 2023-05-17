#ifndef HAARD_COMPOUND_STATEMENT_SCOPE_BUILDER_H
#define HAARD_COMPOUND_STATEMENT_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "semantic/scope_builder.h"

namespace haard {
    class CompoundStatementScopeBuilder : public ScopeBuilder {
    public:
        CompoundStatementScopeBuilder(ScopeBuilderContext* context);

    public:
        void build_compound_statement(CompoundStatement* stmt);
    };
}

#endif
