#include "semantic/modules_scope_builder.h"
#include "semantic/module_scope_builder.h"

using namespace haard;

void ModulesScopeBuilder::build_modules(Modules* modules) {
    this->modules = modules;

    set_qualified_scopes();
    connect_sibling_scopes();
    define_types();
    link_super_types();
    define_methods();
    /*define_functions();*/
}

void ModulesScopeBuilder::set_qualified_scopes() {
    for (int i = 0; i < modules->modules_count(); ++i) {
        ModuleScopeBuilder builder;

        builder.set_module(modules->get_module(i));
        builder.set_qualified_scope();
    }
}

void ModulesScopeBuilder::connect_sibling_scopes() {
    for (int i = 0; i < modules->modules_count(); ++i) {
        ModuleScopeBuilder builder;

        builder.set_module(modules->get_module(i));
        builder.connect_sibling_scopes();
    }
}

void ModulesScopeBuilder::define_types() {
    for (int i = 0; i < modules->modules_count(); ++i) {
        ModuleScopeBuilder builder;

        builder.set_module(modules->get_module(i));
        builder.define_types();
    }
}

void ModulesScopeBuilder::define_methods() {
    for (int i = 0; i < modules->modules_count(); ++i) {
        ModuleScopeBuilder builder;

        builder.set_module(modules->get_module(i));
        builder.define_methods();
    }
}

void ModulesScopeBuilder::define_functions() {
   /* for (int i = 0; i < modules->modules_count(); ++i) {
        ModuleScopeBuilder builder(get_logger());
        builder.define_functions(modules->get_module(i));
    }*/
}

void ModulesScopeBuilder::build_functions() {
    /*for (int i = 0; i < modules->modules_count(); ++i) {
        ModuleScopeBuilder builder(get_logger());
        builder.build_functions(modules->get_module(i));
    }*/
}

void ModulesScopeBuilder::link_super_types() {
    for (int i = 0; i < modules->modules_count(); ++i) {
        ModuleScopeBuilder builder;

        builder.set_module(modules->get_module(i));
        builder.link_super_types();
    }
}
