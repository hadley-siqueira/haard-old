#ifndef HAARD_SCOPE_BUILDER_H
#define HAARD_SCOPE_BUILDER_H

#include <stack>
#include "ast/ast.h"
#include "scope/scope.h"

namespace haard {
    class ScopeBuilder {
        public:
            ScopeBuilder();

        public:
            void build_sources(Sources* sources);
            void build_source(Source* source);

            void build_class(Class* klass);

            void build_function(Function* func);
            void build_function_parameters(Function* func);
            void build_parameter(Variable* var);

        private:
            void enter_scope(Scope* scope);
            void leave_scope();

        private:
            int pass;
            Scope* current_scope;
            std::stack<Scope*> scopes;
    };
}

#endif
