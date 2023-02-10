#ifndef HAARD_MODULE_FUNCTIONS_DEFINER_H
#define HAARD_MODULE_FUNCTIONS_DEFINER_H

#include "ast/module.h"
#include "log/logger.h"
#include "semantic/scope_builder.h"

namespace haard {
    class ModuleFunctionsDefiner : public ScopeBuilder {
    public:
        ModuleFunctionsDefiner(Logger* logger=nullptr);

    public:
        void define_functions(Module* module);
        void define_function(Function* function);

        void define_classes_methods();
        void define_datas_methods();
        void define_structs_methods();
        void define_enums_methods();
        void define_unions_methods();

        void define_compound_methods(CompoundTypeDescriptor* obj);
        void define_method(Function* method);
    };
}

#endif
