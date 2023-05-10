#ifndef HAARD_MODULE_FUNCTIONS_DEFINER_H
#define HAARD_MODULE_FUNCTIONS_DEFINER_H

#include "ast/module.h"
#include "log/logger.h"
#include "semantic/scope_builder.h"

namespace haard {
    class ModuleFunctionsDefiner : public ScopeBuilder {
    public:
        ModuleFunctionsDefiner(ScopeBuilderContext* context=nullptr);

    public:
        void define_functions();
        void define_function(Function* function);
    };
}

#endif
