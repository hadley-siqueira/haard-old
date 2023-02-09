#include "semantic/scope_builder.h"

using namespace haard;


ScopeBuilder::ScopeBuilder() {
    logger = nullptr;
    scope = nullptr;
}

Logger *ScopeBuilder::get_logger() const {
    return logger;
}

void ScopeBuilder::set_logger(Logger* value) {
    logger = value;
}

Scope* ScopeBuilder::get_scope() const {
    return scope;
}

void ScopeBuilder::set_scope(Scope* value) {
    scope = value;
}

Module *ScopeBuilder::get_module() const
{
    return module;
}

void ScopeBuilder::set_module(Module *value)
{
    module = value;
}
