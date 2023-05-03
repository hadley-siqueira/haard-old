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

ModuleScopeBuilder::ModuleScopeBuilder() {
    context = new ScopeBuilderContext();
}

ModuleScopeBuilder::~ModuleScopeBuilder() {
    delete context;
}

void ModuleScopeBuilder::set_qualified_scope() {
    Module* module = get_module();

    module->get_scope()->set_qualified(module->get_relative_path() + ".");
}

void ModuleScopeBuilder::define_types() {
    ModuleTypesDefiner builder(get_context());

    builder.define_types();
}

void ModuleScopeBuilder::define_methods() {
    ModuleMethodsDefiner builder(get_context());

    builder.define_methods();
}

void ModuleScopeBuilder::define_functions(Module* module) {
    ModuleFunctionsDefiner builder(get_context());

    builder.define_functions(module);
}

void ModuleScopeBuilder::link_super_types() {
    Module* module = context->get_module();

    for (int i = 0; i < module->classes_count(); ++i) {
        CompoundTypeDescriptor* decl = module->get_class(i);

        TypeDescriptorLink linker(get_context());
        linker.link_type(decl->get_super_type());
    }

    for (int i = 0; i < module->enums_count(); ++i) {
        CompoundTypeDescriptor* decl = module->get_enum(i);
        /*TypeDescriptorLink linker(module->get_scope(), get_logger());
        linker.link_type(decl->get_super_type());*/
    }
}

void ModuleScopeBuilder::build_functions(Module* module) {
    /*ModuleFunctionsBuilder builder(get_logger());

    builder.build_functions(module);*/
}

void ModuleScopeBuilder::connect_sibling_scopes() {
    Module* module = get_module();
    Scope* scope = module->get_scope();

    for (int i = 0; i < module->import_count(); ++i) {
        Import* import = module->get_import(i);
        scope->add_sibling(import->get_module()->get_scope());
    }
}
