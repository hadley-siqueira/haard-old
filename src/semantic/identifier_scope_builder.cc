#include <iostream>
#include "semantic/identifier_scope_builder.h"

using namespace haard;


IdentifierScopeBuilder::IdentifierScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void IdentifierScopeBuilder::build_identifier(Identifier* id) {
    std::cout << "building identifier\n";
}
