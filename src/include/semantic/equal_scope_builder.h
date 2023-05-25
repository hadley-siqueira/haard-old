#ifndef HAARD_EQUAL_SCOPE_BUILDER_H
#define HAARD_EQUAL_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "semantic/scope_builder.h"

namespace haard {
    class EqualScopeBuilder : public ScopeBuilder {
    public:
        EqualScopeBuilder(ScopeBuilderContext* context);

    public:
        void build_equal(Equal* bin);
    };
}

#endif
