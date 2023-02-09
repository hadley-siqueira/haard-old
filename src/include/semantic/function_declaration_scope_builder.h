#ifndef HAARD_FUNCTION_DECLARATION_SCOPE_BUILDER_H
#define HAARD_FUNCTION_DECLARATION_SCOPE_BUILDER_H

#include "ast/function.h"
#include "scope/scope.h"
#include "log/logger.h"

namespace haard {
    class FunctionDeclarationScopeBuilder {
    public:
        FunctionDeclarationScopeBuilder(Logger* logger=nullptr);

    public:
        void define_function(Function* function);
        void define_template_header(Function* function);
        void define_parameters(Function* function);
        void define_parameter(Variable* param);
        void define_self_type(Function* function);

    private:
        Scope* scope;
        Logger* logger;
    };
}

#endif
