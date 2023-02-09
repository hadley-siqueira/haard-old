#ifndef HAARD_SCOPE_BUILDER_H
#define HAARD_SCOPE_BUILDER_H

#include "log/logger.h"
#include "scope/scope.h"
#include "ast/module.h"

namespace haard {
    class ScopeBuilder {
    public:
        ScopeBuilder();

    public:
        Logger* get_logger() const;
        void set_logger(Logger* value);

        Scope* get_scope() const;
        void set_scope(Scope *value);

        Module* get_module() const;
        void set_module(Module* value);

    protected:
        Logger* logger;
        Scope* scope;
        Module* module;

    };
}

#endif
