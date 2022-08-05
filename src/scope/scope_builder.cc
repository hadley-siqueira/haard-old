#include <iostream>
#include "scope/scope_builder.h"

using namespace haard;

ScopeBuilder::ScopeBuilder() {
    pass = 0;
    current_scope = nullptr;
}

void ScopeBuilder::build_sources(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        build_source(sources->get_source(i));
    }

    pass++;
}

void ScopeBuilder::build_source(Source* source) {
    current_scope = source->get_scope();

    if (pass == 0) {
        for (int i = 0; i < source->classes_count(); ++i) {
            build_class(source->get_class(i));
        }

        for (int i = 0; i < source->function_count(); ++i) {
            build_function(source->get_function(i));
        }
    }
}

void ScopeBuilder::build_class(Class* klass) {
    if (pass == 0) {
        if (!current_scope->has(klass->get_name())) {
            current_scope->define(klass);
        } else {
            Symbol* sym = current_scope->has(klass->get_name());
            std::cout << "Error: you tried to define a class named '" << klass->get_name() << "', but it is already defined. First occurrence\n";
        }
    }
}

void ScopeBuilder::build_function(Function* func) {
    Symbol* sym = current_scope->has(func->get_name());

    if (!sym) {
        current_scope->define(func);
    } else if (sym->get_kind() == SYM_FUNCTION) {
        sym->add_descriptor(func);
    } else {
        std::cout << "Error: you tried to define a function named '" << func->get_name() << "', but it is already defined. First occurrence\n";
    }

    enter_scope(func->get_scope());
    build_function_parameters(func);


    leave_scope();
}

void ScopeBuilder::build_function_parameters(Function* func) {
    Variable* param;

    for (int i = 0; i < func->parameters_count(); ++i) {
        build_variable(func->get_parameter(i));
    }
}

void ScopeBuilder::build_variable(Variable* var) {
    Symbol* sym;

    sym = current_scope->has(var->get_name());

    if (!sym) {
        current_scope->define(var);
    } else if (sym->get_kind() != SYM_VARIABLE) {
        current_scope->define(var);
    } else {
        std::cout << "variable already defined\n";
    }
}

void ScopeBuilder::enter_scope(Scope* scope) {
    scopes.push(current_scope);
    scope->set_parent(current_scope);
    current_scope = scope;
}

void ScopeBuilder::leave_scope() {
    current_scope = scopes.top();
    scopes.pop();
}
