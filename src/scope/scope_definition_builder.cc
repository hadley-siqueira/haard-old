#include <iostream>
#include "scope/scope_definition_builder.h"
#include "log/info_messages.h"
#include "log/error_messages.h"

using namespace haard;

ScopeDefinitionBuilder::ScopeDefinitionBuilder() {
    logger = nullptr;
    current_scope = nullptr;
}

void ScopeDefinitionBuilder::define_sources(Sources* sources) {
    connect_sibling_scopes(sources);
    define_sources_classes(sources);

    for (int i = 0; i < sources->sources_count(); ++i) {
std::cout << "i = " << i << '\n';
        define_source_functions(sources->get_source(i));
    }
}

void ScopeDefinitionBuilder::define_sources_classes(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        define_source_classes(sources->get_source(i));
    }
}

void ScopeDefinitionBuilder::define_source(Source* source) {
    logger->info(info_message_defining_source(source));

    enter_scope(source->get_scope());

    define_source_classes(source);
    define_source_functions(source);

    leave_scope();
}

void ScopeDefinitionBuilder::define_source_classes(Source* source) {
    enter_scope(source->get_scope());

    for (int i = 0; i < source->classes_count(); ++i) {
        define_class(source->get_class(i));
    }

    leave_scope();
}

void ScopeDefinitionBuilder::define_source_functions(Source* source) {
    for (int i = 0; i < source->function_count(); ++i) {
        define_function(source->get_function(i));
    }
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
