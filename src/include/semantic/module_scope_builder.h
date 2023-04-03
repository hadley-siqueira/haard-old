#ifndef HAARD_MODULE_SCOPE_BUILDER_H
#define HAARD_MODULE_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "ast/module.h"
#include "log/logger.h"
#include "semantic/scope_builder.h"

namespace haard {
    class ModuleScopeBuilder : public ScopeBuilder {
    public:
        ModuleScopeBuilder();
        ~ModuleScopeBuilder();

    public:
        void set_qualified_scope();

        void define_types();
        void define_functions(Module* module);
        void define_methods(Module* module);
        void link_super_types(Module* module);
        void build_functions(Module* module);

        void connect_sibling_scopes();
    };
}

#endif
