#include <iostream>
#include "semantic/assignment_scope_builder.h"

using namespace haard;


AssignmentScopeBuilder::AssignmentScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void AssignmentScopeBuilder::build_assignment(BinOp* bin) {
    std::cout << "=\n";
}
