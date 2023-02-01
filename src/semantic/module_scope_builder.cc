#include "semantic/module_scope_builder.h"

using namespace haard;

void ModuleScopeBuilder::build_module(Source* module) {
    build_classes(module);
}

void ModuleScopeBuilder::build_classes(Source* module) {
    for (int i = 0; i < module->classes_count(); ++i) {
        build_class(module->get_class(i));
    }
}

void ModuleScopeBuilder::build_datas(Source* module) {
    for (int i = 0; i < module->data_count(); ++i) {
        build_data(module->get_data(i));
    }
}

void ModuleScopeBuilder::build_enums(Source* module) {

}

void ModuleScopeBuilder::build_unions(Source* module) {

}

void ModuleScopeBuilder::build_structs(Source* module) {

}

void ModuleScopeBuilder::build_class(Class* decl) {
    std::string name = decl->get_qualified_name();

    if (current_scope->local_has(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_class(name, decl);
        logger->info("file.hd: declaring class " + name);
    }
}

void ModuleScopeBuilder::build_data(Data* decl) {
    std::string name = decl->get_qualified_name();

    if (current_scope->local_has(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_data(name, decl);
        logger->info("file.hd: declaring data " + name);
    }
}
