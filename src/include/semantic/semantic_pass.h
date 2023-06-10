#ifndef SEMANTIC_PASS_H
#define SEMANTIC_PASS_H

#include "ast/ast.h"
#include "scope/scope.h"

namespace haard {
    class SemanticPass {
    public:
    private:
        Scope* scope;
        Function* function;
        Module* module;
        Modules* modules;
        Class* klass;
    };
}

#endif
