#ifndef HAARD_MODULE_FUNCTIONS_BUILDER_H
#define HAARD_MODULE_FUNCTIONS_BUILDER_H

#include "ast/module.h"
#include "log/logger.h"
#include "semantic/scope_builder.h"

namespace haard {
    class ModuleFunctionsBuilder : public ScopeBuilder {
    public:
        ModuleFunctionsBuilder(Logger* logger=nullptr);

    public:
        void build_functions(Module* module);
        void build_function(Function* function);

    };
}

#endif
