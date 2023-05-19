#ifndef HAARD_IDENTIFIER_SCOPE_BUILDER_H
#define HAARD_IDENTIFIER_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "semantic/scope_builder.h"

namespace haard {
    class IdentifierScopeBuilder : public ScopeBuilder {
    public:
        IdentifierScopeBuilder(ScopeBuilderContext* context);

    public:
        void build_identifier(Identifier* id);

    };
}

#endif
