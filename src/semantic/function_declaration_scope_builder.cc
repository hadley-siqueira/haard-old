#include "semantic/function_declaration_scope_builder.h"
#include "semantic/type_descriptor_linker.h"
#include "log/info_messages.h"

using namespace haard;

FunctionDeclarationScopeBuilder::FunctionDeclarationScopeBuilder(Logger *logger) {
    this->logger = logger;
}

void FunctionDeclarationScopeBuilder::define_function(Function* function) {
    scope = function->get_scope();
    define_template_header(function);
    define_parameters(function);

    std::string name = function->get_qualified_name();
    scope = function->get_scope()->get_parent();

    if (scope->resolve_local(name)) {
        logger->error_and_exit(name + " already defined");
    } else {
        scope->define_function(name, function);
        logger->info(info_message_define_function(function));
    }
}

void FunctionDeclarationScopeBuilder::define_template_header(Function* function) {
    TypeList* templates = function->get_template_header();

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
    }
}

void FunctionDeclarationScopeBuilder::define_parameters(Function* function) {
    for (int i = 0; i < function->parameters_count(); ++i) {
        define_parameter(function->get_parameter(i));
    }
}

void FunctionDeclarationScopeBuilder::define_parameter(Variable* param) {
    std::string name = param->get_name();
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
    linker.link_type(param->get_type());
}
