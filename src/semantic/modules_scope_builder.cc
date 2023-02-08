#include "semantic/modules_scope_builder.h"
#include "semantic/module_declaration_scope_builder.h"

using namespace haard;

void ModulesScopeBuilder::build_modules(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        ModuleDeclarationScopeBuilder builder;

        builder.set_logger(get_logger());
        builder.build_module(modules->get_module(i));
    }
}

Logger* ModulesScopeBuilder::get_logger() const {
    return logger;
}

void ModulesScopeBuilder::set_logger(Logger* value) {
    logger = value;
}
