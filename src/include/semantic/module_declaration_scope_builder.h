#ifndef HAARD_MODULE_SCOPE_BUILDER_H
#define HAARD_MODULE_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "ast/source.h"
#include "log/logger.h"

namespace haard {
    class ModuleDeclarationScopeBuilder {
        public:
            void build_module(Module* module);

            void define_types(Module* module);
            void define_functions(Module* module);
            void define_methods(Module* module);

            void define_classes(Module* module);
            void define_datas(Module* module);
            void define_enums(Module* module);
            void define_unions(Module* module);
            void define_structs(Module* module);

            void define_classes_methods(Module* module);
            void define_class_method(Class* decl);

            void define_class(Class* decl);
            void define_data(Data* decl);
            void define_enum(Enum* decl);
            void define_union(Union* decl);
            void define_struct(Struct* decl);
            void define_function(Function* decl);

            void define_type(CompoundTypeDescriptor* decl, int kind, std::string msg);

        public:
            Logger* get_logger() const;
            void set_logger(Logger* value);

        private:
            Module* module;
            Scope* current_scope;
            Logger* logger;
    };
}

#endif
