#ifndef HAARD_MODULE_SCOPE_BUILDER_H
#define HAARD_MODULE_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "ast/source.h"
#include "log/logger.h"

namespace haard {
    class ModuleDeclarationScopeBuilder {
        public:
            void build_module(Source* module);
            void define_classes(Source* module);
            void define_datas(Source* module);
            void define_enums(Source* module);
            void define_unions(Source* module);
            void define_structs(Source* module);
            void define_functions(Source* module);

            void define_class(Class* decl);
            void define_data(Data* decl);
            void define_enum(Enum* decl);
            void define_union(Union* decl);
            void define_struct(Struct* decl);
            void define_function(Function* decl);

            void define_type(TypeDeclaration* decl, int kind, std::string msg);

        public:
            Logger* get_logger() const;
            void set_logger(Logger* value);

        private:
            Source* module;
            Scope* current_scope;
            Logger* logger;
    };
}

#endif
