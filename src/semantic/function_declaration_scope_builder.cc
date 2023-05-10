#include "semantic/function_declaration_scope_builder.h"
#include "semantic/type_descriptor_linker.h"
#include "log/log_messages.h"
#include "log/info_messages.h"
#include <iostream>

using namespace haard;

FunctionDeclarationScopeBuilder::FunctionDeclarationScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void FunctionDeclarationScopeBuilder::define_function(Function* function) {
    set_function(function);
    enter_scope(function->get_scope());
    define_template_header(function);
    define_parameters(function);
    define_self_type(function);

    std::string name = function->get_qualified_name();
    leave_scope();

    if (get_scope()->resolve_local(name)) {
        log_error_and_exit(name + " already defined");
    } else {
        get_scope()->define_function(name, function);
        log_info(info_message_define_function(function));
    }

    set_function(nullptr);
}

void FunctionDeclarationScopeBuilder::define_template_header(Function* function) {
    TemplateHeader* templates = function->get_template_header();

    if (templates == nullptr) {
        return;
    }

    if (function->is_template()) {
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
}

void FunctionDeclarationScopeBuilder::define_parameters(Function* function) {
    for (int i = 0; i < function->parameters_count(); ++i) {
        define_parameter(function->get_parameter(i));
    }
}

void FunctionDeclarationScopeBuilder::define_parameter(Variable* param) {
    std::string name = param->get_name();
    Symbol* sym = get_scope()->resolve_local(name);

    if (!sym) {
        get_scope()->define_parameter(name, param);
    } else if (sym->get_kind() != SYM_PARAMETER) {
        get_scope()->define_parameter(name, param);
    } else {
        std::string msg = "<red>error: </red>parameter '" + name + "' already defined";
        log_error_and_exit(msg);
    }

    TypeDescriptorLink linker(get_context());
    linker.link_type(param->get_type());
}

void FunctionDeclarationScopeBuilder::define_self_type(Function* function) {
    TypeDescriptorLink linker(get_context());
    FunctionType* ftype = new FunctionType();

    if (function->get_template_header()) {
        TemplateHeader* header = function->get_template_header();

        for (int i = 0; i < header->types_count(); ++i) {
            ftype->add_template(header->get_type(i));
        }
    }

    if (function->parameters_count() > 0) {
        for (int i = 0; i < function->parameters_count(); ++i) {
            ftype->add_param_type(function->get_parameter(i)->get_type());
        }
    } else {
        ftype->add_param_type(new Type(TYPE_VOID));
    }

    linker.link_type(function->get_return_type());
    ftype->set_return_type(function->get_return_type());
    function->set_self_type(ftype);
    linker.link_type(ftype);
}
