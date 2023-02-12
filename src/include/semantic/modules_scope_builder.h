#ifndef HAARD_MODULES_SCOPE_BUILDER_H
#define HAARD_MODULES_SCOPE_BUILDER_H

#include "ast/modules.h"
#include "log/logger.h"
#include "semantic/scope_builder.h"

namespace haard {
    class ModulesScopeBuilder : public ScopeBuilder {
    public:
        void build_modules(Modules* modules);

        void set_qualified_scopes();
        void connect_sibling_scopes();
        void define_types();
        void define_methods();
        void define_functions();

        void build_functions();

        void link_super_types();

    private:
        Modules* modules;
    };
}

#endif
