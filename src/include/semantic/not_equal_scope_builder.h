#ifndef HAARD_NOT_EQUAL_SCOPE_BUILDER_H
#define HAARD_NOT_EQUAL_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "semantic/scope_builder.h"

namespace haard {
    class NotEqualScopeBuilder : public ScopeBuilder {
    public:
        NotEqualScopeBuilder(ScopeBuilderContext* context);

    public:
        void build_not_equal(NotEqual* bin);
    };
}

#endif
