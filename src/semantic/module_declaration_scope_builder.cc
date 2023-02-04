#include "semantic/module_declaration_scope_builder.h"

using namespace haard;

void ModuleDeclarationScopeBuilder::build_module(Source* module) {
    current_scope = module->get_scope();

    build_classes(module);
    build_datas(module);
    build_structs(module);
    build_enums(module);
    build_unions(module);
}

void ModuleDeclarationScopeBuilder::build_classes(Source* module) {
    for (int i = 0; i < module->classes_count(); ++i) {
        build_class(module->get_class(i));
    }
}

void ModuleDeclarationScopeBuilder::build_datas(Source* module) {
    for (int i = 0; i < module->data_count(); ++i) {
        build_data(module->get_data(i));
    }
}

void ModuleDeclarationScopeBuilder::build_enums(Source* module) {
    for (int i = 0; i < module->enums_count(); ++i) {
        build_enum(module->get_enum(i));
    }
}

void ModuleDeclarationScopeBuilder::build_unions(Source* module) {
    for (int i = 0; i < module->unions_count(); ++i) {
        build_union(module->get_union(i));
    }
}

void ModuleDeclarationScopeBuilder::build_structs(Source* module) {
    for (int i = 0; i < module->structs_count(); ++i) {
        build_struct(module->get_struct(i));
    }
}

void ModuleDeclarationScopeBuilder::build_class(Class* decl) {
    std::string name = decl->get_qualified_name();

    if (current_scope->local_has(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_class(name, decl);
        logger->info("file.hd: declaring class " + name);
    }
}

void ModuleDeclarationScopeBuilder::build_data(Data* decl) {
    std::string name = decl->get_qualified_name();

    if (current_scope->local_has(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_data(name, decl);
        logger->info("file.hd: declaring data " + name);
    }
}

void ModuleDeclarationScopeBuilder::build_enum(Enum* decl) {
    std::string name = decl->get_qualified_name();

    if (current_scope->local_has(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_enum(name, decl);
        logger->info("file.hd: declaring enum " + name);
    }
}

void ModuleDeclarationScopeBuilder::build_union(Union* decl) {
    std::string name = decl->get_qualified_name();

    if (current_scope->local_has(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_union(name, decl);
        logger->info("file.hd: declaring union " + name);
    }
}

void ModuleDeclarationScopeBuilder::build_struct(Struct* decl) {
    std::string name = decl->get_qualified_name();

    if (current_scope->local_has(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_struct(name, decl);
        logger->info("file.hd: declaring struct " + name);
    }
}

Logger* ModuleDeclarationScopeBuilder::get_logger() const {
    return logger;
}

void ModuleDeclarationScopeBuilder::set_logger(Logger* value) {
    logger = value;
}
