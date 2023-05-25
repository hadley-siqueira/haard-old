#include <iostream>
#include "semantic/equal_scope_builder.h"
#include "semantic/expression_scope_builder.h"

using namespace haard;

EqualScopeBuilder::EqualScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void EqualScopeBuilder::build_equal(Equal* bin) {
    ExpressionScopeBuilder b1(get_context());
    ExpressionScopeBuilder b2(get_context());

    b1.build_expression(bin->get_left());
    b2.build_expression(bin->get_right());

    bin->set_type(new Type(TYPE_BOOL));
}
