#include "semantic/scope_builder_context.h"

using namespace haard;

ScopeBuilderContext::ScopeBuilderContext() {
    module = nullptr;
    function = nullptr;
    compound = nullptr;
    scope = nullptr;
}

Module* ScopeBuilderContext::get_module() const {
    return module;
}

void ScopeBuilderContext::set_module(Module* value) {
    module = value;
}

Function* ScopeBuilderContext::get_function() const {
    return function;
}

void ScopeBuilderContext::set_function(Function* value) {
    function = value;
}

CompoundTypeDescriptor* ScopeBuilderContext::get_compound() const {
    return compound;
}

void ScopeBuilderContext::set_compound(CompoundTypeDescriptor* value) {
    compound = value;
}

Scope* ScopeBuilderContext::get_scope() const {
    return scope;
}

void ScopeBuilderContext::set_scope(Scope* value) {
    scope = value;
}

