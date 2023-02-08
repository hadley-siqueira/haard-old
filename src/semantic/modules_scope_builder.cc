#include "semantic/modules_scope_builder.h"
#include "semantic/module_scope_builder.h"

using namespace haard;

void ModulesScopeBuilder::build_modules(Modules* modules) {
    this->modules = modules;

    set_qualified_scopes();
    connect_sibling_scopes();
    define_types();
    declare_functions();
}

void ModulesScopeBuilder::set_qualified_scopes() {
    for (int i = 0; i < modules->modules_count(); ++i) {
        ModuleScopeBuilder builder(get_logger());
        builder.set_qualified_scope(modules->get_module(i));
    }
}

void ModulesScopeBuilder::connect_sibling_scopes() {
    for (int i = 0; i < modules->modules_count(); ++i) {
        ModuleScopeBuilder builder(get_logger());
        builder.connect_sibling_scopes(modules->get_module(i));
    }
}

void ModulesScopeBuilder::define_types() {
    for (int i = 0; i < modules->modules_count(); ++i) {
        ModuleScopeBuilder builder(get_logger());
        builder.define_types(modules->get_module(i));
    }
}

void ModulesScopeBuilder::declare_functions() {
    for (int i = 0; i < modules->modules_count(); ++i) {
        ModuleScopeBuilder builder(get_logger());
        builder.define_functions(modules->get_module(i));
    }
}

Logger* ModulesScopeBuilder::get_logger() const {
    return logger;
}

void ModulesScopeBuilder::set_logger(Logger* value) {
    logger = value;
}
