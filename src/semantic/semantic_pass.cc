#include "semantic/semantic_pass.h"
#include "semantic/type_descriptor_linker.h"
#include "log/actions.h"

using namespace haard;

SemanticPass::SemanticPass() {
    scope = nullptr;
    function = nullptr;
    var_counter = 0;
}

void SemanticPass::enter_scope(Scope* scope) {
    scope->set_parent(get_scope());
    set_scope(scope);
}

void SemanticPass::leave_scope() {
    set_scope(get_scope()->get_parent());
}

void SemanticPass::link_type(Type* type) {
    TypeDescriptorLink linker(get_scope());

    linker.link_type(type);
}

Scope* SemanticPass::get_scope() const {
    return scope;
}

void SemanticPass::set_scope(Scope* value) {
    scope = value;
}

Function* SemanticPass::get_function() const {
    return function;
}

void SemanticPass::set_function(Function* value) {
    function = value;
}

void SemanticPass::reset_local_var_counter() {
    var_counter = 0;
}

int SemanticPass::next_local_var_counter() {
    return var_counter++;
}

CompoundTypeDescriptor *SemanticPass::get_compound() const {
    return compound;
}

void SemanticPass::set_compound(CompoundTypeDescriptor* newCompound) {
    compound = newCompound;
}
