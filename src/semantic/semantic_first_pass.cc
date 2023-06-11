#include <iostream>
#include "semantic/semantic_first_pass.h"
#include "log/actions.h"

using namespace haard;

void SemanticFirstPass::build_modules(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        build_module(modules->get_module(i));
    }
}

void SemanticFirstPass::build_module(Module* module) {
    set_qualified_scope(module);
    connect_sibling_scopes(module);
    define_types(module);
}

void SemanticFirstPass::set_qualified_scope(Module* module) {
    module->get_scope()->set_qualified(module->get_relative_path() + ".");
}

void SemanticFirstPass::connect_sibling_scopes(Module* module) {
    Import* import;
    Scope* scope = module->get_scope();

    for (int i = 0; i < module->import_count(); ++i) {
        import = module->get_import(i);
        scope->add_sibling(import->get_module()->get_scope());
    }
}

void SemanticFirstPass::define_types(Module* module) {
    CompoundTypeDescriptor* decl;

    for (int i = 0; i < module->classes_count(); ++i) {
        decl = module->get_class(i);
        define_type(decl, SYM_CLASS, "class");
    }
}

void SemanticFirstPass::define_type(CompoundTypeDescriptor* decl, int kind, std::string msg) {
    NamedType* self_type = new NamedType();
    std::string qname = decl->get_name();


    define_template_header(decl);

    std::string name = decl->get_qualified_name();

    if (get_scope()->resolve_local(name)) {
        log_error_and_exit(name + " already defined");
    } else {
        get_scope()->define_type(kind, name, decl);
        //log_info(info_message_define_type(decl, msg));
        DBG; exit(0);
    }

    self_type->set_name(qname);
    link_type(self_type);
}

void SemanticFirstPass::define_template_header(CompoundTypeDescriptor* decl) {
    TemplateHeader* templates = decl->get_template_header();

    if (templates == nullptr) {
        return;
    }

    enter_scope(decl->get_scope());

    if (decl->is_template()) {
        for (int i = 0; i < templates->types_count(); ++i) {
            NamedType* named = (NamedType*) templates->get_type(i);
            std::string name = named->get_name();
            get_scope()->define_template(name, i);
            link_type(named);
        }
    } else {
        for (int i = 0; i < templates->types_count(); ++i) {
            link_type(templates->get_type(i));
        }
    }

    leave_scope();
}
