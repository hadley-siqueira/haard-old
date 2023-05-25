#include <iostream>
#include "semantic/not_equal_scope_builder.h"
#include "semantic/expression_scope_builder.h"

using namespace haard;

NotEqualScopeBuilder::NotEqualScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void NotEqualScopeBuilder::build_not_equal(NotEqual* bin) {
    ExpressionScopeBuilder b1(get_context());
    ExpressionScopeBuilder b2(get_context());

    b1.build_expression(bin->get_left());
    b2.build_expression(bin->get_right());

    bin->set_type(new Type(TYPE_BOOL));
}
