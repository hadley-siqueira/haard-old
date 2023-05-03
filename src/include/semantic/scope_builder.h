#ifndef HAARD_SCOPE_BUILDER_H
#define HAARD_SCOPE_BUILDER_H

#include "scope/scope.h"
#include "ast/module.h"
#include "semantic/scope_builder_context.h"

namespace haard {
    class ScopeBuilder {
    public:
        ScopeBuilder();

    public:
        void enter_scope(Scope* scope);
        void leave_scope();

        Scope* get_scope() const;
        void set_scope(Scope *value);

        Module* get_module() const;
        void set_module(Module* value);

        Function* get_function() const;
        void set_function(Function* value);

        ScopeBuilderContext* get_context() const;
        void set_context(ScopeBuilderContext* value);

    protected:
        ScopeBuilderContext* context;
    };
}

#endif
