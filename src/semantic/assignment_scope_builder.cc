#include <iostream>
#include "semantic/assignment_scope_builder.h"
#include "semantic/expression_scope_builder.h"

using namespace haard;


AssignmentScopeBuilder::AssignmentScopeBuilder(ScopeBuilderContext* context) {
    set_context(context);
}

void AssignmentScopeBuilder::build_assignment(BinOp* bin) {
    ExpressionScopeBuilder b1(get_context());
    ExpressionScopeBuilder b2(get_context());

    bool new_var = false;

    b1.build_expression(bin->get_right());

    if (is_new_var_assignment(bin)) {
        new_var = true;
        create_new_var(bin);
    }

    b2.build_expression(bin->get_left());

    /*if (new_var && is_constructor_call_assignment(bin)) {
        bin->set_kind(EXPR_CONSTRUCTOR_ASSIGNMENT);
    }*/
}

bool AssignmentScopeBuilder::is_new_var_assignment(BinOp* bin) {
    if (bin->get_left()->get_kind() != EXPR_ID) {
        return false;
    }

    Identifier* id = (Identifier*) bin->get_left();
    Symbol* sym = get_scope()->has(id->get_name());

    return sym == nullptr;
}

void AssignmentScopeBuilder::create_new_var(BinOp* bin) {
    Identifier* id = (Identifier*) bin->get_left();

    Variable* var = new Variable(id);
    var->set_type(bin->get_right()->get_type());
    var->set_uid(get_next_var_counter());
    var->set_kind(VAR_LOCAL);
    bin->set_initial_assign(true);
    get_scope()->define(var);
    get_function()->add_variable(var);
}
