#ifndef HAARD_SEMANTIC_PASS_H
#define HAARD_SEMANTIC_PASS_H

#include "ast/ast.h"
#include "scope/scope.h"

namespace haard {
    class SemanticPass {
    public:
        SemanticPass();

    public:
        void enter_scope(Scope* scope);
        void leave_scope();

        void link_type(Type* type);

        Scope* get_scope() const;
        void set_scope(Scope* value);

        Function* get_function() const;
        void set_function(Function* value);

    private:
        Scope* scope;
        Function* function;
    };
}

#endif
