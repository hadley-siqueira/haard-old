#include "semantic/function_declaration_scope_builder.h"
#include "semantic/type_descriptor_linker.h"
#include "log/info_messages.h"
#include <iostream>

using namespace haard;

FunctionDeclarationScopeBuilder::FunctionDeclarationScopeBuilder(Logger *logger) {
    //set_logger(logger);
}

void FunctionDeclarationScopeBuilder::define_function(Function* function) {
    /*set_scope(function->get_scope());
    define_template_header(function);
    define_parameters(function);
    define_self_type(function);

    std::string name = function->get_qualified_name();
    set_scope(function->get_scope()->get_parent());

    if (scope->resolve_local(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        scope->define_function(name, function);
        logger->info(info_message_define_function(function));
    }*/
}

void FunctionDeclarationScopeBuilder::define_template_header(Function* function) {
    /*TemplateHeader* templates = function->get_template_header();

    if (templates == nullptr) {
        return;
    }

    if (function->is_template()) {
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
    }*/
}

void FunctionDeclarationScopeBuilder::define_parameters(Function* function) {
    for (int i = 0; i < function->parameters_count(); ++i) {
        define_parameter(function->get_parameter(i));
    }
}

void FunctionDeclarationScopeBuilder::define_parameter(Variable* param) {
    /*std::string name = param->get_name();
    Symbol* sym = scope->resolve_local(name);

    if (!sym) {
        scope->define_parameter(name, param);
    } else if (sym->get_kind() != SYM_PARAMETER) {
        scope->define_parameter(name, param);
    } else {
        std::string msg = "<red>error: </red>parameter '" + name + "' already defined";
        logger->error_and_exit(msg);
    }

    TypeDescriptorLink linker(scope, logger);
    linker.link_type(param->get_type());*/
}

void FunctionDeclarationScopeBuilder::define_self_type(Function* function) {
    /*TypeDescriptorLink linker(scope, logger);
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
    linker.link_type(ftype);*/
}
