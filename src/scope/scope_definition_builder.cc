#include <iostream>
#include <cstring>
#include "scope/scope_definition_builder.h"
#include "log/info_messages.h"
#include "log/error_messages.h"

using namespace haard;

ScopeDefinitionBuilder::ScopeDefinitionBuilder() {
    logger = nullptr;
    current_scope = nullptr;
}

void ScopeDefinitionBuilder::build(Sources* sources) {
    define_sources(sources);
    build_sources(sources);
}

void ScopeDefinitionBuilder::build_sources(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        build_source(sources->get_source(i));
    }
}

void ScopeDefinitionBuilder::build_source(Source* source) {
    enter_scope(source->get_scope());

    for (int i = 0; i < source->classes_count(); ++i) {
        build_class(source->get_class(i));
    }

    for (int i = 0; i < source->function_count(); ++i) {
        build_function(source->get_function(i));
    }

    leave_scope();
}

void ScopeDefinitionBuilder::build_class(Class* klass) {
    enter_scope(klass->get_scope());

    define_class_template_header(klass);
    define_class_super(klass);
    define_class_variables(klass);
    define_class_methods(klass);
    build_class_methods(klass);

    leave_scope();
}

void ScopeDefinitionBuilder::build_class_methods(Class* klass) {

}

void ScopeDefinitionBuilder::build_function(Function* function) {

}

void ScopeDefinitionBuilder::define_sources(Sources* sources) {
    connect_sibling_scopes(sources);

    define_sources_classes(sources);
    define_sources_functions(sources);
}

void ScopeDefinitionBuilder::define_sources_classes(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        define_source_classes(sources->get_source(i));
    }
}

void ScopeDefinitionBuilder::define_sources_functions(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        define_source_functions(sources->get_source(i));
    }
}

void ScopeDefinitionBuilder::define_source_classes(Source* source) {
    enter_scope(source->get_scope());

    for (int i = 0; i < source->classes_count(); ++i) {
        define_class(source->get_class(i));
    }

    leave_scope();
}

void ScopeDefinitionBuilder::define_source_functions(Source* source) {
    enter_scope(source->get_scope());

    for (int i = 0; i < source->function_count(); ++i) {
        define_function(source->get_function(i));
    }

    leave_scope();
}

void ScopeDefinitionBuilder::define_class(Class* klass) {
    Class* other;
    Symbol* sym;

    logger->info(info_message_defining_class(klass));
    sym = current_scope->local_has(klass->get_name());

    if (!sym) {
        current_scope->define(klass);
    } else {
        logger->error(error_message_cant_define_class(klass, sym));
    }
}

void ScopeDefinitionBuilder::define_class_variables(Class* klass) {
    for (int i = 0; i < klass->variables_count(); ++i) {
        define_class_variable(klass->get_variable(i));
        klass->get_variable(i)->set_uid(i); 
    }
}

void ScopeDefinitionBuilder::define_class_variable(Variable* var) {
    Symbol* sym;

    sym = current_scope->has_field(var->get_name());

    if (!sym) {
        current_scope->define(SYM_CLASS_VARIABLE, var);
        link_type(var->get_type());
    } else if (sym->get_kind() != SYM_CLASS_VARIABLE) {
        current_scope->define(SYM_CLASS_VARIABLE, var);
        link_type(var->get_type());
    } else {
        logger->error("<red>error: </red> class variable already defined");
    }
}

void ScopeDefinitionBuilder::define_class_methods(Class* klass) {
    for (int i = 0; i < klass->methods_count(); ++i) {
        define_class_method(klass->get_method(i));
    }
}

void ScopeDefinitionBuilder::define_class_method(Function* method) {
    Symbol* sym = current_scope->has_field(method->get_name());

    define_function_signature(method);

    if (!sym) {
        logger->info(info_message_defining_function(method));
        current_scope->define(method);
    } else if (sym->get_kind() == SYM_METHOD) {
        logger->info(info_message_defining_function(method));
        define_overloaded_function(sym, method);
    } else {
        logger->error("can't define function");
    }
}

void ScopeDefinitionBuilder::define_class_template_header(Class* klass) {

}

void ScopeDefinitionBuilder::define_class_super(Class* klass) {
    if (klass->has_super_class()) {
        link_type(klass->get_super_class());
    }
}

void ScopeDefinitionBuilder::connect_sibling_scopes(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        Source* src = sources->get_source(i);
        Scope* scope = src->get_scope();

        for (int j = 0; j < src->import_count(); ++j) {
            scope->add_sibling(src->get_import(j)->get_source()->get_scope());
        }
    }
}

void ScopeDefinitionBuilder::define_function(Function* function) {
    Symbol* sym = current_scope->local_has(function->get_name());

    define_function_signature(function);

    if (!sym) {
        logger->info(info_message_defining_function(function));
        current_scope->define(function);
    } else if (sym->get_kind() == SYM_FUNCTION) {
        logger->info(info_message_defining_function(function));
        define_overloaded_function(sym, function);
    } else {
        logger->error("can't define function");
    }
}

void ScopeDefinitionBuilder::define_function_signature(Function* function) {
    enter_scope(function->get_scope());

    define_function_template_list(function);
    define_function_parameters(function);
    link_type(function->get_return_type());
    define_function_self_type(function);

    leave_scope();
}

void ScopeDefinitionBuilder::define_function_template_list(Function* function) {
    Symbol* sym;
    TypeList* types = function->get_template_list();

    if (types) {
        for (int i = 0; i < types->types_count(); ++i) {
            TemplateType* type = (TemplateType*) types->get_type(i);
            sym = current_scope->local_has(type->get_name());

            if (!sym) {
                current_scope->define(type);
                link_type(type->get_bind_type());
            } else {
                logger->error("<red>error: </red>template name already defined");
            }
        }
    }
}

void ScopeDefinitionBuilder::define_function_parameters(Function* function) {
    Variable* param;
    Symbol* sym;

    for (int i = 0; i < function->parameters_count(); ++i) {
        param = function->get_parameter(i);
        param->set_uid(i);
        link_type(param->get_type());

        sym = current_scope->has(param->get_name());

        if (!sym) {
            current_scope->define(SYM_PARAMETER, param);
        } else if (sym->get_kind() != SYM_PARAMETER) {
            current_scope->define(SYM_PARAMETER, param);
        } else {
            logger->error("<red>error: </red>parameter already defined");
        }
    }
}

void ScopeDefinitionBuilder::define_function_self_type(Function* function) {
    TypeList* types = new TypeList(TYPE_FUNCTION);

    if (function->get_template_list()) {
        TypeList* ts = function->get_template_list();

        for (int i = 0; i < ts->types_count(); ++i) {
            types->add_template(ts->get_type(i));
        }
    }

    if (function->parameters_count() > 0) {
        for (int i = 0; i < function->parameters_count(); ++i) {
            types->add_type(function->get_parameter(i)->get_type());
        }
    } else {
        types->add_type(new Type(TYPE_VOID));
    }

    types->add_type(function->get_return_type());
    function->set_self_type(types);
}

void ScopeDefinitionBuilder::define_overloaded_function(Symbol* symbol, Function* function) {
    for (int i = 0; i < symbol->overloaded_count(); ++i) {
        Function* other = (Function*) symbol->get_descriptor(i);

        if (function->same_signature(other)) {
            logger->error("<red>error: </red>function with same signature");
        }
    }

    symbol->add_descriptor(function);
    function->set_overloaded_index(symbol->overloaded_count() - 1);
}

void ScopeDefinitionBuilder::link_type(Type* type) {
    IndirectionType* it = (IndirectionType*) type;

    if (type == nullptr) {
        return;
    }

    switch (type->get_kind()) {
    case TYPE_NAMED:
        link_named_type((NamedType*) type);
        break;

    case TYPE_POINTER:
    case TYPE_REFERENCE:
        link_type(it->get_subtype());
        break;
    }
}

void ScopeDefinitionBuilder::link_named_type(NamedType* type) {
    
}

void ScopeDefinitionBuilder::set_logger(Logger* logger) {
    this->logger = logger;
}

// PRIVATE
void ScopeDefinitionBuilder::enter_scope(Scope* scope) {
    scopes.push(current_scope);
    scope->set_parent(current_scope);
    current_scope = scope;
}

void ScopeDefinitionBuilder::leave_scope() {
    current_scope = scopes.top();
    scopes.pop();
}
