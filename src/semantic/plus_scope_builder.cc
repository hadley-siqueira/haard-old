#include "semantic/plus_scope_builder.h"
#include "semantic/expression_scope_builder.h"

using namespace haard;

PlusScopeBuilder::PlusScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void PlusScopeBuilder::build_plus(BinOp* bin) {
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


