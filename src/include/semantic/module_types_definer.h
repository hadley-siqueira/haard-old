#ifndef HAARD_MODULE_TYPES_DEFINER_H
#define HAARD_MODULE_TYPES_DEFINER_H

#include "ast/module.h"
#include "log/logger.h"
#include "semantic/scope_builder.h"

namespace haard {
    class ModuleTypesDefiner : public ScopeBuilder {
    public:
        ModuleTypesDefiner(ScopeBuilderContext* context=nullptr);

    public:
        void define_types(Module* module);

        Logger* get_logger() const;
        void set_logger(Logger* value);

    private:
        void define_classes();
        void define_datas();
        void define_enums();
        void define_unions();
        void define_structs();

        void define_class(Class* decl);
        void define_data(Data* decl);
        void define_enum(Enum* decl);
        void define_union(Union* decl);
        void define_struct(Struct* decl);

        void define_type(CompoundTypeDescriptor* decl, int kind, std::string msg);
    };
}

#endif
