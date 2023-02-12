#ifndef HAARD_MODULE_FUNCTIONS_DEFINER_H
#define HAARD_MODULE_FUNCTIONS_DEFINER_H

#include "ast/module.h"
#include "log/logger.h"
#include "semantic/scope_builder.h"

namespace haard {
    class ModuleFunctionsDefiner : public ScopeBuilder {
    public:
        ModuleFunctionsDefiner(Logger* logger=nullptr);

    public:
        void define_functions(Module* module);
        void define_function(Function* function);
    };
}

#endif
