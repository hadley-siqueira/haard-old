#ifndef HAARD_SCOPE_DEFINITION_BUILDER_H
#define HAARD_SCOPE_DEFINITION_BUILDER_H

#include <stack>
#include "ast/ast.h"
#include "log/logger.h"
#include "scope/scope.h"

namespace haard {
    class ScopeDefinitionBuilder {
        public:
            ScopeDefinitionBuilder();

        public:
            void define_sources(Sources* sources);
            void define_source(Source* source);
            void define_source_classes(Source* source);
            void define_class(Class* klass);

        public:
            void set_logger(Logger* logger);

        private:
            void enter_scope(Scope* scope);
            void leave_scope();

        private:
            Logger* logger;
            Scope* current_scope;
            std::stack<Scope*> scopes;
    };
}

#endif
