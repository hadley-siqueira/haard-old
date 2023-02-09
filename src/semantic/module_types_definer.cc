#include "semantic/module_types_definer.h"
#include "semantic/type_descriptor_linker.h"
#include "log/info_messages.h"

using namespace haard;

ModuleTypesDefiner::ModuleTypesDefiner(Logger* logger) {
    set_logger(logger);
}

void ModuleTypesDefiner::define_types(Module* module) {
    set_module(module);
    set_scope(module->get_scope());

    define_classes();
    define_datas();
    define_structs();
    define_enums();
    define_unions();
}

Logger* ModuleTypesDefiner::get_logger() const {
    return logger;
}

void ModuleTypesDefiner::set_logger(Logger* value) {
    logger = value;
}

void ModuleTypesDefiner::define_classes() {
    for (int i = 0; i < module->classes_count(); ++i) {
        define_class(module->get_class(i));
    }
}

void ModuleTypesDefiner::define_datas() {
    for (int i = 0; i < module->data_count(); ++i) {
        define_data(module->get_data(i));
    }
}

void ModuleTypesDefiner::define_enums() {
    for (int i = 0; i < module->enums_count(); ++i) {
        define_enum(module->get_enum(i));
    }
}

void ModuleTypesDefiner::define_unions() {
    for (int i = 0; i < module->unions_count(); ++i) {
        define_union(module->get_union(i));
    }
}

void ModuleTypesDefiner::define_structs() {
    for (int i = 0; i < module->structs_count(); ++i) {
        define_struct(module->get_struct(i));
    }
}

void ModuleTypesDefiner::define_class(Class* decl) {
    define_type(decl, SYM_CLASS, "class");
}

void ModuleTypesDefiner::define_data(Data* decl) {
    define_type(decl, SYM_DATA, "data");
}

void ModuleTypesDefiner::define_enum(Enum* decl) {
    define_type(decl, SYM_ENUM, "enum");
}

void ModuleTypesDefiner::define_union(Union* decl) {
    define_type(decl, SYM_UNION, "union");
}

void ModuleTypesDefiner::define_struct(Struct* decl) {
    define_type(decl, SYM_STRUCT, "struct");
}

void ModuleTypesDefiner::define_type(CompoundTypeDescriptor* decl, int kind, std::string msg) {
    auto old_scope = scope;
    scope = decl->get_scope();
    scope->set_parent(old_scope);
    NamedType* self_type = new NamedType();
    std::string qname = decl->get_name();

    TemplateHeader* templates = decl->get_template_header();

    if (templates != nullptr) {
        if (decl->is_template()) {
            for (int i = 0; i < templates->types_count(); ++i) {
                NamedType* named = (NamedType*) templates->get_type(i);
                std::string name = named->get_name();
                scope->define_template(name, i);
                TypeDescriptorLink linker(scope, logger);
                linker.link_type(named);
            }
        } else {
            for (int i = 0; i < templates->types_count(); ++i) {
                TypeDescriptorLink linker(scope, logger);
                linker.link_type(templates->get_type(i));
            }
        }

        qname += templates->get_qualified_name();
    }

    scope = old_scope;
    std::string name = decl->get_qualified_name();

    if (scope->resolve_local(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        scope->define_type(kind, name, decl);
        logger->info(info_message_define_type(decl, msg));
    }

    TypeDescriptorLink linker(scope, logger);
    self_type->set_name(qname);
    linker.link_type(self_type);
}
