#include "semantic/module_declaration_scope_builder.h"
#include "semantic/type_descriptor_linker.h"
#include "semantic/function_declaration_scope_builder.h"
#include "log/messages.h"
#include "log/info_messages.h"

using namespace haard;

void ModuleDeclarationScopeBuilder::build_module(Source* module) {
    this->module = module;
    current_scope = module->get_scope();
    current_scope->set_qualified(module->get_relative_path() + ".");

    define_types(module);
    define_methods(module);
    define_functions(module);
}

void ModuleDeclarationScopeBuilder::define_types(Source* module) {
    define_classes(module);
    define_datas(module);
    define_structs(module);
    define_enums(module);
    define_unions(module);
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

void ModuleDeclarationScopeBuilder::define_classes_methods(Source* module) {
    for (int i = 0; i < module->classes_count(); ++i) {
        define_class_method(module->get_class(i));
    }
}

void ModuleDeclarationScopeBuilder::define_class_method(Class* decl) {
    for (int i = 0; i < decl->methods_count(); ++i) {
        define_function(decl->get_method(i));
    }
}

void ModuleDeclarationScopeBuilder::define_functions(Source* module) {
    for (int i = 0; i < module->functions_count(); ++i) {
        define_function(module->get_function(i));
    }
}

void ModuleDeclarationScopeBuilder::define_methods(Source* module) {
    define_classes_methods(module);
}

void ModuleDeclarationScopeBuilder::define_class(Class* decl) {
    define_type(decl, SYM_CLASS, "class");
}

void ModuleDeclarationScopeBuilder::define_data(Data* decl) {
    define_type(decl, SYM_DATA, "data");
}

void ModuleDeclarationScopeBuilder::define_enum(Enum* decl) {
    define_type(decl, SYM_ENUM, "enum");
}

void ModuleDeclarationScopeBuilder::define_union(Union* decl) {
    define_type(decl, SYM_UNION, "union");
}

void ModuleDeclarationScopeBuilder::define_struct(Struct* decl) {
    define_type(decl, SYM_STRUCT, "struct");
}

void ModuleDeclarationScopeBuilder::define_function(Function* decl) {
    FunctionDeclarationScopeBuilder builder(logger);
    builder.define_function(decl);
}

void ModuleDeclarationScopeBuilder::define_type(CompoundTypeDescriptor* decl, int kind, std::string msg) {
    auto old_scope = current_scope;
    current_scope = decl->get_scope();
    current_scope->set_parent(old_scope);
    TypeList* templates = decl->get_template_header();

    if (templates != nullptr) {
        if (decl->is_template()) {
            for (int i = 0; i < templates->types_count(); ++i) {
                NamedType* named = (NamedType*) templates->get_type(i);
                std::string name = named->get_name();
                current_scope->define_template(name, i);
                TypeDescriptorLink linker(current_scope, logger);
                linker.link_type(named);
            }
        } else {
            for (int i = 0; i < templates->types_count(); ++i) {
                TypeDescriptorLink linker(current_scope, logger);
                linker.link_type(templates->get_type(i));
            }
        }
    }

    current_scope = old_scope;
    std::string name = decl->get_qualified_name();

    if (current_scope->resolve_local(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        current_scope->define_type(kind, name, decl);
        logger->info(info_message_define_type(decl, msg));
    }
}

Logger* ModuleDeclarationScopeBuilder::get_logger() const {
    return logger;
}

void ModuleDeclarationScopeBuilder::set_logger(Logger* value) {
    logger = value;
}
