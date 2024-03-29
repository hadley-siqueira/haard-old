#include <iostream>
#include "semantic/semantic_first_pass.h"
#include "log/actions.h"
#include "log/infos.h"
#include "log/errors.h"
#include "parser/parser.h"

using namespace haard;

void SemanticFirstPass::build_modules(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        build_module(modules->get_module(i));
    }
}

void SemanticFirstPass::build_module(Module* module) {
    enter_scope(module->get_scope());

    module->get_scope()->set_qualified(module->get_relative_path() + ".");
    build_imports(module);
    define_user_types(module);
    build_classes(module);
    build_structs(module);
    build_unions(module);
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

void SemanticFirstPass::build_unions(Module* module) {

}

void SemanticFirstPass::define_user_types(Module* module) {
    for (int i = 0; i < module->classes_count(); ++i) {
        define_user_type(module->get_class(i));
    }

    for (int i = 0; i < module->structs_count(); ++i) {
        define_user_type(module->get_struct(i));
    }

    for (int i = 0; i < module->enums_count(); ++i) {
        define_user_type(module->get_enum(i));
    }

    for (int i = 0; i < module->unions_count(); ++i) {
        define_user_type(module->get_union(i));
    }
}

void SemanticFirstPass::build_functions(Module* module) {
    for (int i = 0; i < module->functions_count(); ++i) {
        build_function(module->get_function(i));
    }
}

void SemanticFirstPass::build_fields(CompoundTypeDescriptor* decl) {
    enter_scope(decl->get_scope());

    for (int i = 0; i < decl->fields_count(); ++i) {
        Variable* field = decl->get_field(i);

        if (get_scope()->resolve_field(field->get_name())) {
            log_error_and_exit("field already declared!");
        }

        get_scope()->define_field(decl->get_field(i));
        link_type(decl->get_field(i)->get_type());
        decl->get_field(i)->set_uid(i);
        decl->get_field(i)->set_kind(VAR_FIELD);
    }

    leave_scope();
}

void SemanticFirstPass::build_methods(CompoundTypeDescriptor* decl) {
    enter_scope(decl->get_scope());

    for (int i = 0; i < decl->methods_count(); ++i) {
        build_method(decl->get_method(i));
    }

    add_default_constructor(decl);
    add_default_destructor(decl);

    leave_scope();
}

void SemanticFirstPass::build_import(Import* import) {
    get_scope()->add_sibling(import->get_module()->get_scope());
}

void SemanticFirstPass::build_class(Class* decl) {
    enter_scope(decl->get_scope());
    set_compound(decl);

    if (decl->is_template()) {
        leave_scope();
        return;
    }

    if (decl->get_super_type()) {
        Type* t = decl->get_super_type();
        link_type(t);
        decl->get_scope()->set_super(t->get_scope());
    }

    leave_scope();

    build_fields(decl);
    build_methods(decl);
}

void SemanticFirstPass::build_struct(Struct* decl) {
    DBG; exit(0);
}

void SemanticFirstPass::build_function(Function* function) {
    Symbol* sym;
    set_function(function);
    enter_scope(function->get_scope());

    build_template_header(function->get_template_header());
    build_parameters(function);
    link_type(function->get_return_type());

    leave_scope();

    std::string name = function->get_name();

    sym = get_scope()->resolve_local(name);

    if (sym) {
        for (int i = 0; i < sym->descriptors_count(); ++i) {
            SymbolDescriptor* desc = sym->get_descriptor(i);

            if (desc->get_kind() == SYM_FUNCTION) {
                Function* other = (Function*) desc->get_descriptor();

                if (other->parameters_count() == function->parameters_count()) {
                    int count = function->parameters_count();
                    bool equal_types = true;

                    for (int i = 0; i < count; ++i) {
                        Type* t1 = function->get_parameter(i)->get_type();
                        Type* t2 = other->get_parameter(i)->get_type();

                        if (!t1->equal(t2)) {
                            equal_types = false;
                        }
                    }

                    if (equal_types) {
                        log_error_and_exit("function already defined with same param types");
                    }
                }

                // if (other->equals(function)
                if (false) {
                    log_error_and_exit(name + " already defined");
                }
            }
        }
    }

    get_scope()->define_function(function);

    if (logging_info()) {
        log_info(info_define_function(function));
    }

    set_function(nullptr);
}

void SemanticFirstPass::build_method(Function* method) {
    Symbol* sym;
    set_function(method);
    enter_scope(method->get_scope());
    method->set_module(get_compound()->get_module());

    build_template_header(method->get_template_header());
    build_parameters(method);

    leave_scope();

    std::string name = method->get_name();

    sym = get_scope()->resolve_local(name);

    if (sym) {
        for (int i = 0; i < sym->descriptors_count(); ++i) {
            SymbolDescriptor* desc = sym->get_descriptor(i);

            if (desc->get_kind() == SYM_METHOD) {
                Function* other = (Function*) desc->get_descriptor();

                if (other->parameters_count() == method->parameters_count()) {
                    int count = method->parameters_count();
                    bool equal_types = true;

                    for (int i = 0; i < count; ++i) {
                        Type* t1 = method->get_parameter(i)->get_type();
                        Type* t2 = other->get_parameter(i)->get_type();

                        if (!t1->equal(t2)) {
                            equal_types = false;
                        }
                    }

                    if (equal_types) {
                        log_error_and_exit("method already defined with same param types");
                    }
                }

                // if (other->equals(function)
                if (false) {
                    log_error_and_exit(name + " already defined");
                }
            }
        }
    }

    get_scope()->define_method(method);

    if (logging_info()) {
        log_info(info_define_method(method));
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

    if (sym) {
        for (int i = 0; i < sym->descriptors_count(); ++i) {
            SymbolDescriptor* desc = sym->get_descriptor(i);

            if (desc->get_kind() == SYM_PARAMETER) {
                std::string msg = "parameter '" + name + "' already defined. Line " + get_function()->get_name();
                msg += param->get_line();
                log_info(get_scope()->debug());
                log_info(get_scope()->get_parent()->debug());
                log_error_and_exit(msg);
            }
        }
    }

    get_scope()->define_parameter(param);
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

            if (get_scope()->resolve_local(name)) {
                log_error_and_exit("template already defined");
            }

            get_scope()->define_template(named);
            link_type(named);
        }
    } else {
        for (int i = 0; i < templates->types_count(); ++i) {
            link_type(templates->get_type(i));
        }
    }
}

void SemanticFirstPass::build_self_type(CompoundTypeDescriptor* desc) {
    NamedType* named = new NamedType();
    named->set_name(desc->get_name());

    link_type(named);
    desc->set_self_type(named);
}

void SemanticFirstPass::add_default_constructor(CompoundTypeDescriptor* decl) {
    Function* f;
    Symbol* sym = get_scope()->resolve_local("init");

    if (sym != nullptr) {
        for (int i = 0; i < sym->descriptors_count(); ++i) {
            SymbolDescriptor* desc = sym->get_descriptor(i);
            f = (Function*) desc->get_descriptor();

            if (f->parameters_count() == 0) {
                return;
            }
        }
    }

    log_info("Creating default constructor for " + decl->get_name());
    std::string cons = "def init : void\n    return\n\n";
    Parser p;
    f = p.read_function_from_string(cons);

    decl->add_method(f);
    build_method(f);
}

void SemanticFirstPass::add_default_destructor(CompoundTypeDescriptor* decl) {
    Function* f;
    Symbol* sym = get_scope()->resolve_local("destroy");

    if (sym != nullptr) {
        return;
    }

    log_info("Creating default destructor for " + decl->get_name());
    std::string cons = "def destroy : void\n    return\n\n";
    Parser p;
    f = p.read_function_from_string(cons);

    decl->add_method(f);
    build_method(f);
}

void SemanticFirstPass::define_user_type(CompoundTypeDescriptor* decl) {
    int kind = SYM_CLASS;
    std::string name = decl->get_name();

    enter_scope(decl->get_scope());
    build_template_header(decl->get_template_header());
    leave_scope();

    switch (decl->get_kind()) {
    case DECL_CLASS:
        kind = SYM_CLASS;
        break;

    case DECL_ENUM:
        kind = SYM_ENUM;
        break;

    case DECL_STRUCT:
        kind = SYM_STRUCT;
        break;

    case DECL_UNION:
        kind = SYM_UNION;
        break;
    }

    Declaration* flag = check_for_redefinition(decl);

    if (flag) {
        log_error_and_exit(error_redefinition(decl, flag));
    }

    get_scope()->define(kind, name, decl);
    build_self_type(decl);

    if (logging_info()) {
        log_info(info_define_user_type(decl));
    }
}

Declaration* SemanticFirstPass::check_for_redefinition(Declaration* decl) {
    SymbolDescriptor* desc;
    Symbol* sym = get_scope()->resolve(decl->get_name());

    if (sym == nullptr) {
        return nullptr;
    }

    for (int i = 0; i < sym->descriptors_count(); ++i) {
        desc = sym->get_descriptor(i);
        Declaration* odecl = (Declaration*) desc->get_descriptor();

        if (redefines(decl, odecl)) {
            return odecl;
        }
    }

    return nullptr;
}

bool SemanticFirstPass::redefines(Declaration* d1, Declaration* d2) {
    if (d1->get_name() != d2->get_name()) {
        return false;
    }

    if (d1->is_type_declaration() && d2->is_type_declaration()) {
        if (d1->get_template_header() && d2->get_template_header()) {
            int c1 = d1->get_template_header()->types_count();
            int c2 = d2->get_template_header()->types_count();

            if (c1 != c2) {
                return false;
            }
        } else if (d1->get_template_header() || d2->get_template_header()) {
            return false;
        } else {
            std::cout << d1->get_template_header() << ' ' << d2->get_template_header() << '\n';
            std::cout << d1->get_name() << ' ' << d2->get_name() << '\n';
            DBG; exit(0);
        }
    } else if (d1->is_function() && d2->is_function()) {
        Function* f1 = (Function*) d1;
        Function* f2 = (Function*) d2;

        if (f1->parameters_count() != f2->parameters_count()) {
            return false;
        }

        for (int i = 0; i < f1->parameters_count(); ++i) {
            Type* t1 = f1->get_parameter(i)->get_type();
            Type* t2 = f2->get_parameter(i)->get_type();

            if (!t1->equal(t2)) {
                return false;
            }
        }
    }

    return true;
}
