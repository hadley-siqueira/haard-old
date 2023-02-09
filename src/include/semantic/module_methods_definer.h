#ifndef HAARD_MODULE_METHODS_DEFINER_H
#define HAARD_MODULE_METHODS_DEFINER_H

#include "ast/module.h"
#include "log/logger.h"

namespace haard {
    class ModuleMethodsDefiner {
    public:
        ModuleMethodsDefiner(Logger* logger=nullptr);

    public:
        void define_methods(Module* module);

        void define_classes_methods();
        void define_datas_methods();
        void define_structs_methods();
        void define_enums_methods();
        void define_unions_methods();

        void define_compound_methods(CompoundTypeDescriptor* obj);
        void define_method(Function* method);

    public:
        Logger* get_logger() const;
        void set_logger(Logger* value);

    private:
        Scope* scope;
        Logger* logger;
        Module* module;
    };
}

#endif
