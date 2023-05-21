#ifndef HAARD_PLUS_SCOPE_BUILDER_H
#define HAARD_PLUS_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "semantic/scope_builder.h"

namespace haard {
    class PlusScopeBuilder : public ScopeBuilder {
    public:
        PlusScopeBuilder(ScopeBuilderContext* context);

    public:
        void build_plus(BinOp* bin);
    };
}

#endif
