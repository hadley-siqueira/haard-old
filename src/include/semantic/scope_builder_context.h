#ifndef HAARD_SCOPE_BUILDER_CONTEXT_H
#define HAARD_SCOPE_BUILDER_CONTEXT_H

#include "ast/module.h"
#include "ast/function.h"
#include "ast/compound_type_descriptor.h"

namespace haard {
    class ScopeBuilderContext {
    public:
        ScopeBuilderContext();

        Module* get_module() const;
        void set_module(Module* value);

        Function* get_function() const;
        void set_function(Function* value);

        CompoundTypeDescriptor* get_compound() const;
        void set_compound(CompoundTypeDescriptor* value);

        Scope* get_scope() const;
        void set_scope(Scope* value);

        int get_var_counter() const;
        void set_var_counter(int value);

    private:
        Module* module;
        Function* function;
        CompoundTypeDescriptor* compound;
        Scope* scope;
        int var_counter;
    };
}

#endif
