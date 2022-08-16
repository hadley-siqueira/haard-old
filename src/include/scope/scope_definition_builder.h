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
            void build(Sources* sources);

        private:
            void build_sources(Sources* sources);
            void build_source(Source* source);

            void build_class(Class* klass);
            void build_class_methods(Class* klass);

            void build_function(Function* function);

        private:
            void define_sources(Sources* sources);
            void define_sources_classes(Sources* sources);
            void define_sources_functions(Sources* sources);
            void define_source_classes(Source* source);
            void define_source_functions(Source* source);

            void define_class(Class* klass);
            void define_class_variables(Class* klass);
            void define_class_variable(Variable* var);
            void define_class_methods(Class* klass);
            void define_class_method(Function* method);
            void define_class_template_header(Class* klass);
            void define_class_super(Class* klass);

            void define_function(Function* function);
            void define_function_signature(Function* function);
            void define_function_template_list(Function* function);
            void define_function_parameters(Function* function);
            void define_function_self_type(Function* function);
            void define_overloaded_function(Symbol* symbol, Function* function);

            void connect_sibling_scopes(Sources* sources);

        public:
            void link_type(Type* type);
            void link_named_type(NamedType* type);

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
