#include <iostream>
#include "semantic/minus_scope_builder.h"
#include "semantic/expression_scope_builder.h"

using namespace haard;

MinusScopeBuilder::MinusScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void MinusScopeBuilder::build_minus(Minus* bin) {
    ExpressionScopeBuilder b1(get_context());
    ExpressionScopeBuilder b2(get_context());

    b1.build_expression(bin->get_left());
    b2.build_expression(bin->get_right());

    Type* tleft = bin->get_left()->get_type();
    Type* tright = bin->get_right()->get_type();

    tleft = tleft->promote(tright);

    // FIXME
    bin->set_type(tleft);
}


