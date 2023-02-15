#ifndef HAARD_SCOPE_BUILDER_H
#define HAARD_SCOPE_BUILDER_H

#include "log/logger.h"
#include "scope/scope.h"
#include "ast/module.h"
#include "semantic/scope_builder_context.h"

namespace haard {
    class ScopeBuilder {
    public:
        ScopeBuilder();

    public:
        void copy_state(ScopeBuilder* other);
        void enter_scope(Scope* scope);
        void leave_scope();

        Logger* get_logger() const;
        void set_logger(Logger* value);

        Scope* get_scope() const;
        void set_scope(Scope *value);

        Module* get_module() const;
        void set_module(Module* value);

        ScopeBuilderContext* get_context() const;
        void set_context(ScopeBuilderContext* value);

    protected:
        ScopeBuilderContext* context;
    };
}

#endif
