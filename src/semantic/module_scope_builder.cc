#include "semantic/module_scope_builder.h"
#include "semantic/type_descriptor_linker.h"
#include "semantic/function_declaration_scope_builder.h"
#include "log/messages.h"
#include "log/info_messages.h"
#include "semantic/module_types_definer.h"
#include "semantic/module_methods_definer.h"
#include "semantic/module_functions_definer.h"
#include "semantic/module_functions_builder.h"
#include <iostream>

using namespace haard;

ModuleScopeBuilder::ModuleScopeBuilder(Logger* logger) {
    set_logger(logger);
}

void ModuleScopeBuilder::set_qualified_scope(Module* module) {
    module->get_scope()->set_qualified(module->get_relative_path() + ".");
}

void ModuleScopeBuilder::define_types(Module* module) {
    ModuleTypesDefiner builder(get_context());

    builder.define_types(module);
}

void ModuleScopeBuilder::define_methods(Module* module) {
    ModuleMethodsDefiner builder(get_context());

    builder.define_methods(module);
}

void ModuleScopeBuilder::define_functions(Module* module) {
    ModuleFunctionsDefiner builder(get_context());

    builder.define_functions(module);
}

void ModuleScopeBuilder::link_super_types(Module* module) {
    for (int i = 0; i < module->classes_count(); ++i) {
        CompoundTypeDescriptor* decl = module->get_class(i);
        TypeDescriptorLink linker(module->get_scope(), logger);
        linker.link_type(decl->get_super_type());
    }

    for (int i = 0; i < module->enums_count(); ++i) {
        CompoundTypeDescriptor* decl = module->get_enum(i);
        TypeDescriptorLink linker(module->get_scope(), logger);
        linker.link_type(decl->get_super_type());
    }
}

void ModuleScopeBuilder::build_functions(Module* module) {
    ModuleFunctionsBuilder builder(logger);

    builder.build_functions(module);
}

void ModuleScopeBuilder::connect_sibling_scopes(Module* module) {
    Scope* scope = module->get_scope();

    for (int i = 0; i < module->import_count(); ++i) {
        Import* import = module->get_import(i);
        scope->add_sibling(import->get_module()->get_scope());
    }
}
