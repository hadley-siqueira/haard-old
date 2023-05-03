#ifndef HAARD_MODULE_METHODS_DEFINER_H
#define HAARD_MODULE_METHODS_DEFINER_H

#include "ast/module.h"
#include "log/logger.h"
#include "semantic/scope_builder.h"

namespace haard {
    class ModuleMethodsDefiner : public ScopeBuilder {
    public:
        ModuleMethodsDefiner(ScopeBuilderContext* context=nullptr);

    public:
        void define_methods();

        void define_classes_methods();
        void define_structs_methods();
        void define_enums_methods();
        void define_unions_methods();

        void define_compound_methods(CompoundTypeDescriptor* obj);
        void define_method(Function* method);
    };
}

#endif
