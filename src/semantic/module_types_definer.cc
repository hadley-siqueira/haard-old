#include "semantic/module_types_definer.h"
#include "semantic/type_descriptor_linker.h"
#include "log/info_messages.h"
#include "log/log_messages.h"

using namespace haard;

ModuleTypesDefiner::ModuleTypesDefiner(ScopeBuilderContext *context) {
    set_context(context);
}

void ModuleTypesDefiner::define_types() {
    set_scope(get_module()->get_scope());

    define_classes();
    define_structs();
    define_enums();
    define_unions();
}

void ModuleTypesDefiner::define_classes() {
    for (int i = 0; i < get_module()->classes_count(); ++i) {
        define_class(get_module()->get_class(i));
    }
}

void ModuleTypesDefiner::define_enums() {
    for (int i = 0; i < get_module()->enums_count(); ++i) {
        define_enum(get_module()->get_enum(i));
    }
}

void ModuleTypesDefiner::define_unions() {
    for (int i = 0; i < get_module()->unions_count(); ++i) {
        define_union(get_module()->get_union(i));
    }
}

void ModuleTypesDefiner::define_structs() {
    for (int i = 0; i < get_module()->structs_count(); ++i) {
        define_struct(get_module()->get_struct(i));
    }
}

void ModuleTypesDefiner::define_class(Class* decl) {
    define_type(decl, SYM_CLASS, "class");
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
    auto old_scope = get_scope();
    set_scope(decl->get_scope());
    get_scope()->set_parent(old_scope);
    //enter_scope(decl->get_scope());
    NamedType* self_type = new NamedType();
    std::string qname = decl->get_name();

    TemplateHeader* templates = decl->get_template_header();

    if (templates != nullptr) {
        if (decl->is_template()) {
            for (int i = 0; i < templates->types_count(); ++i) {
                NamedType* named = (NamedType*) templates->get_type(i);
                std::string name = named->get_name();
                get_scope()->define_template(name, i);
                TypeDescriptorLink linker(get_context());
                linker.link_type(named);
            }
        } else {
            for (int i = 0; i < templates->types_count(); ++i) {
                TypeDescriptorLink linker(get_context());
                linker.link_type(templates->get_type(i));
            }
        }

        qname += templates->get_qualified_name();
    }

    set_scope(old_scope);
    //leave_scope();
    std::string name = decl->get_qualified_name();

    if (get_scope()->resolve_local(name)) {
        log_error_and_exit(name + " already defined");
    } else {
        get_scope()->define_type(kind, name, decl);
        log_info(info_message_define_type(decl, msg));
    }

    TypeDescriptorLink linker(get_context());
    self_type->set_name(qname);
    linker.link_type(self_type);
}
