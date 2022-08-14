#include "scope/scope_definition_builder.h"
#include "log/info_messages.h"
#include "log/error_messages.h"

using namespace haard;

ScopeDefinitionBuilder::ScopeDefinitionBuilder() {
    logger = nullptr;
    current_scope = nullptr;
}

void ScopeDefinitionBuilder::define_sources(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        define_source(sources->get_source(i));
    }
}

void ScopeDefinitionBuilder::define_source(Source* source) {
    logger->info(info_message_defining_source(source));

    enter_scope(source->get_scope());
    define_source_classes(source);
    leave_scope();
}

void ScopeDefinitionBuilder::define_source_classes(Source* source) {
    for (int i = 0; i < source->classes_count(); ++i) {
        define_class(source->get_class(i));
    }
}

void ScopeDefinitionBuilder::define_class(Class* klass) {
    Class* other;
    Symbol* sym;

    logger->info(info_message_defining_class(klass));

    sym = current_scope->has(klass->get_name());

    if (!sym) {
        current_scope->define(klass);
    } else {
        logger->error(error_message_cant_define_class(klass, sym));
    }
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
