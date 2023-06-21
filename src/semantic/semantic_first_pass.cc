#include <iostream>
#include "semantic/semantic_first_pass.h"
#include "log/actions.h"
#include "log/infos.h"

using namespace haard;

void SemanticFirstPass::build_modules(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        build_module(modules->get_module(i));
    }
}

void SemanticFirstPass::build_module(Module* module) {
    enter_scope(module->get_scope());

    module->get_scope()->set_qualified(module->get_relative_path() + ".");
    build_classes(module);
    build_functions(module);

    leave_scope();
}

void SemanticFirstPass::build_imports(Module* module) {
    for (int i = 0; i < module->import_count(); ++i) {
        build_import(module->get_import(i));
    }
}

void SemanticFirstPass::build_classes(Module* module) {
    for (int i = 0; i < module->classes_count(); ++i) {
        build_class(module->get_class(i));
    }
}

void SemanticFirstPass::build_structs(Module* module) {
    for (int i = 0; i < module->structs_count(); ++i) {
        build_struct(module->get_struct(i));
    }
}

void SemanticFirstPass::build_functions(Module* module) {
    for (int i = 0; i < module->functions_count(); ++i) {
        build_function(module->get_function(i));
    }
}

void SemanticFirstPass::build_import(Import* import) {
    get_scope()->add_sibling(import->get_module()->get_scope());
}

void SemanticFirstPass::build_class(Class* decl) {
    enter_scope(decl->get_scope());
    build_template_header(decl->get_template_header());
    leave_scope();

    std::string name = decl->get_qualified_name();

    if (get_scope()->resolve_local(name)) {
        log_error_and_exit(name + " already defined");
    } else {
        get_scope()->define_class(decl);

        if (logging_info()) {
            log_info(info_define_class(decl));
        }
    }
}

void SemanticFirstPass::build_struct(Struct* decl) {

}

void SemanticFirstPass::build_function(Function* function) {
    set_function(function);
    enter_scope(function->get_scope());

    build_template_header(function->get_template_header());
    build_parameters(function);
    //build_function_self_type(function);

    leave_scope();

    std::string name = function->get_qualified_name();

    if (get_scope()->resolve_local(name)) {
        log_error_and_exit(name + " already defined");
    } else {
        get_scope()->define_function(name, function);

        if (logging_info()) {
            log_info(info_define_function(function));
        }
    }

    set_function(nullptr);
}

void SemanticFirstPass::build_parameters(Function* function) {
    for (int i = 0; i < function->parameters_count(); ++i) {
        build_parameter(function->get_parameter(i), i);
    }
}

void SemanticFirstPass::build_parameter(Variable* param, int idx) {
    std::string name = param->get_name();
    Symbol* sym = get_scope()->resolve_local(name);
    param->set_uid(idx);

    if (!sym) {
        get_scope()->define_parameter(name, param);
    } else if (sym->get_kind() != SYM_PARAMETER) {
        get_scope()->define_parameter(name, param);
    } else {
        std::string msg = "parameter '" + name + "' already defined. Line " + get_function()->get_name();
        msg += param->get_line();
        log_info(get_scope()->debug());
        log_info(get_scope()->get_parent()->debug());
        log_error_and_exit(msg);
    }

    link_type(param->get_type());
}

void SemanticFirstPass::build_function_self_type(Function* function) {
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

    link_type(function->get_return_type());
    ftype->set_return_type(function->get_return_type());
    function->set_self_type(ftype);
    link_type(ftype);
}

void SemanticFirstPass::build_template_header(TemplateHeader* templates) {
    if (templates == nullptr) {
        return;
    }

    if (templates->is_template()) {
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
}
