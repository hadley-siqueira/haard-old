#include "semantic/module_methods_definer.h"
#include "semantic/function_declaration_scope_builder.h"

using namespace haard;

ModuleMethodsDefiner::ModuleMethodsDefiner(ScopeBuilderContext* context) {
    set_context(context);
}

void ModuleMethodsDefiner::define_methods() {
    define_classes_methods();
    define_structs_methods();
    define_enums_methods();
    define_unions_methods();
}

void ModuleMethodsDefiner::define_classes_methods() {
    Module* module = get_module();

    for (int i = 0; i < module->classes_count(); ++i) {
        define_compound_methods(module->get_class(i));
    }
}

void ModuleMethodsDefiner::define_structs_methods() {
    Module* module = get_module();

    for (int i = 0; i < module->structs_count(); ++i) {
        define_compound_methods(module->get_struct(i));
    }
}

void ModuleMethodsDefiner::define_enums_methods() {
    Module* module = get_module();

    for (int i = 0; i < module->enums_count(); ++i) {
        define_compound_methods(module->get_enum(i));
    }
}

void ModuleMethodsDefiner::define_unions_methods() {
    Module* module = get_module();

    for (int i = 0; i < module->unions_count(); ++i) {
        define_compound_methods(module->get_union(i));
    }
}

void ModuleMethodsDefiner::define_compound_methods(CompoundTypeDescriptor* decl) {
    enter_scope(decl->get_scope());

    for (int i = 0; i < decl->methods_count(); ++i) {
        define_method(decl->get_method(i));
    }

    leave_scope();
}

void ModuleMethodsDefiner::define_method(Function* method) {
    FunctionDeclarationScopeBuilder builder(get_context());

    builder.define_function(method);
}
