#include "semantic/scope_builder.h"
#include "log/log_messages.h"

using namespace haard;

ScopeBuilder::ScopeBuilder() {
    context = nullptr;
}

void ScopeBuilder::enter_scope(Scope* scope) {
    scope->set_parent(get_scope());
    set_scope(scope);
}

void ScopeBuilder::leave_scope() {
    set_scope(get_scope()->get_parent());
}

Scope* ScopeBuilder::get_scope() const {
    return context->get_scope();
}

void ScopeBuilder::set_scope(Scope* value) {
    context->set_scope(value);
}

Module* ScopeBuilder::get_module() const {
    return context->get_module();
}

void ScopeBuilder::set_module(Module* value) {
    context->set_module(value);
    context->set_scope(value->get_scope());
}

Function *ScopeBuilder::get_function() const {
    return context->get_function();
}

void ScopeBuilder::set_function(Function *value) {
    context->set_function(value);
}

ScopeBuilderContext* ScopeBuilder::get_context() const {
    return context;
}

void ScopeBuilder::set_context(ScopeBuilderContext* value) {
    context = value;
}
