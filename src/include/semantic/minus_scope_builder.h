#ifndef HAARD_MINUS_SCOPE_BUILDER_H
#define HAARD_MINUS_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "semantic/scope_builder.h"

namespace haard {
    class MinusScopeBuilder : public ScopeBuilder {
    public:
        MinusScopeBuilder(ScopeBuilderContext* context);

    public:
        void build_minus(Minus* bin);
    };
}

#endif
