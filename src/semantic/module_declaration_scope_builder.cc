#include "semantic/module_declaration_scope_builder.h"
#include "semantic/type_descriptor_linker.h"

using namespace haard;

void ModuleDeclarationScopeBuilder::build_module(Source* module) {
    current_scope = module->get_scope();
    current_scope->set_qualified(module->get_relative_path() + ".");

    define_classes(module);
    define_datas(module);
    define_structs(module);
    define_enums(module);
    define_unions(module);
    define_functions(module);
}

void ModuleDeclarationScopeBuilder::define_classes(Source* module) {
    for (int i = 0; i < module->classes_count(); ++i) {
        define_class(module->get_class(i));
    }
}

void ModuleDeclarationScopeBuilder::define_datas(Source* module) {
    for (int i = 0; i < module->data_count(); ++i) {
        define_data(module->get_data(i));
    }
}

void ModuleDeclarationScopeBuilder::define_enums(Source* module) {
    for (int i = 0; i < module->enums_count(); ++i) {
        define_enum(module->get_enum(i));
    }
}

void ModuleDeclarationScopeBuilder::define_unions(Source* module) {
    for (int i = 0; i < module->unions_count(); ++i) {
        define_union(module->get_union(i));
    }
}

void ModuleDeclarationScopeBuilder::define_structs(Source* module) {
    for (int i = 0; i < module->structs_count(); ++i) {
        define_struct(module->get_struct(i));
    }
}

void ModuleDeclarationScopeBuilder::define_functions(Source* module) {
    for (int i = 0; i < module->functions_count(); ++i) {
        define_function(module->get_function(i));
    }
}

void ModuleDeclarationScopeBuilder::define_class(Class* decl) {
    std::string name = decl->get_qualified_name();

    if (current_scope->resolve_local(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_class(name, decl);
        logger->info("file.hd: declaring class " + name);
    }
}

void ModuleDeclarationScopeBuilder::define_data(Data* decl) {
    std::string name = decl->get_qualified_name();

    if (current_scope->resolve_local(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_data(name, decl);
        logger->info("file.hd: declaring data " + name);
    }
}

void ModuleDeclarationScopeBuilder::define_enum(Enum* decl) {
    std::string name = decl->get_qualified_name();

    if (current_scope->resolve_local(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_enum(name, decl);
        logger->info("file.hd: declaring enum " + name);
    }
}

void ModuleDeclarationScopeBuilder::define_union(Union* decl) {
    std::string name = decl->get_qualified_name();

    if (current_scope->resolve_local(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_union(name, decl);
        logger->info("file.hd: declaring union " + name);
    }
}

void ModuleDeclarationScopeBuilder::define_struct(Struct* decl) {
    std::string name = decl->get_qualified_name();

    if (current_scope->resolve_local(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_struct(name, decl);
        logger->info("file.hd: declaring struct " + name);
    }
}

void ModuleDeclarationScopeBuilder::define_function(Function* decl) {
    auto old_scope = current_scope;
    decl->get_scope()->set_parent(current_scope);
    current_scope = decl->get_scope();

    if (decl->is_template()) {
        TypeList* templates = decl->get_template_header();

        for (int i = 0; i < templates->types_count(); ++i) {
            NamedType* named = (NamedType*) templates->get_type(i);
            std::string name = named->get_name();
            current_scope->define_template(name, i);
            TypeDescriptorLink linker(current_scope, logger);
            linker.link_type(named);
        }
    }

    for (int i = 0; i < decl->parameters_count(); ++i) {
        TypeDescriptorLink linker(current_scope, logger);

        linker.link_type(decl->get_parameter(i)->get_type());
    }

    current_scope = old_scope;

    std::string name = decl->get_qualified_name();

    if (current_scope->resolve_local(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_function(name, decl);
        logger->info("file.hd: declaring function " + name);
    }
}

Logger* ModuleDeclarationScopeBuilder::get_logger() const {
    return logger;
}

void ModuleDeclarationScopeBuilder::set_logger(Logger* value) {
    logger = value;
}
