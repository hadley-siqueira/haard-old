#ifndef HAARD_MODULE_SCOPE_BUILDER_H
#define HAARD_MODULE_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "ast/module.h"
#include "log/logger.h"

namespace haard {
    class ModuleScopeBuilder {
    public:
        ModuleScopeBuilder(Logger* logger=nullptr);

    public:
        void set_qualified_scope(Module* module);

        void define_types(Module* module);
        void define_functions(Module* module);
        void define_methods(Module* module);
        void link_super_types(Module* module);

        void connect_sibling_scopes(Module* module);
        void define_function(Function* decl);

    public:
        Logger* get_logger() const;
        void set_logger(Logger* value);

    private:
        Scope* current_scope;
        Logger* logger;
    };
}

#endif
