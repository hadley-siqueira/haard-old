#include "semantic/module_scope_builder.h"
#include "semantic/type_descriptor_linker.h"
#include "semantic/function_declaration_scope_builder.h"
#include "log/messages.h"
#include "log/info_messages.h"
#include "semantic/module_types_definer.h"

using namespace haard;

ModuleScopeBuilder::ModuleScopeBuilder(Logger* logger) {
    set_logger(logger);
}

void ModuleScopeBuilder::set_qualified_scope(Module* module) {
    module->get_scope()->set_qualified(module->get_relative_path() + ".");
}

void ModuleScopeBuilder::define_types(Module* module) {
    ModuleTypesDefiner builder(logger);

    builder.define_types(module);
}

void ModuleScopeBuilder::define_classes_methods(Module* module) {
    for (int i = 0; i < module->classes_count(); ++i) {
        define_class_method(module->get_class(i));
    }
}

void ModuleScopeBuilder::define_class_method(Class* decl) {
    for (int i = 0; i < decl->methods_count(); ++i) {
        define_function(decl->get_method(i));
    }
}

void ModuleScopeBuilder::define_functions(Module* module) {
    for (int i = 0; i < module->functions_count(); ++i) {
        define_function(module->get_function(i));
    }
}

void ModuleScopeBuilder::define_methods(Module* module) {
    define_classes_methods(module);
}

void ModuleScopeBuilder::define_function(Function* decl) {
    FunctionDeclarationScopeBuilder builder(logger);
    builder.define_function(decl);
}

Logger* ModuleScopeBuilder::get_logger() const {
    return logger;
}

void ModuleScopeBuilder::set_logger(Logger* value) {
    logger = value;
}

void ModuleScopeBuilder::connect_sibling_scopes(Module* module) {
    Scope* scope = module->get_scope();

    for (int i = 0; i < module->import_count(); ++i) {
        Import* import = module->get_import(i);
        scope->add_sibling(import->get_module()->get_scope());
    }
}
