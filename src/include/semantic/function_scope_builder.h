#ifndef HAARD_FUNCTION_SCOPE_BUILDER_H
#define HAARD_FUNCTION_SCOPE_BUILDER_H

#include "semantic/scope_builder.h"

namespace haard {
    class FunctionScopeBuilder : ScopeBuilder {
    public:
        FunctionScopeBuilder(ScopeBuilderContext* context);

    public:
        void build_function(Function* function);
    };
}

#endif
