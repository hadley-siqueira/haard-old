#ifndef HAARD_MODULE_SCOPE_BUILDER_H
#define HAARD_MODULE_SCOPE_BUILDER_H

#include "ast/ast.h"
#include "ast/source.h"
#include "log/logger.h"

namespace haard {
    class ModuleScopeBuilder {
        public:
            void build_module(Source* module);
            void build_classes(Source* module);
            void build_datas(Source* module);
            void build_enums(Source* module);
            void build_unions(Source* module);
            void build_structs(Source* module);

            void build_class(Class* decl);
            void build_data(Data* decl);
            /*void build_enum(Enum* decl);
            void build_union(Union* decl);
            void build_struct(Struct* decl);*/

        private:
            Scope* current_scope;
            Logger* logger;
    };
}

#endif
