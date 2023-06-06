#include "ast/function_expression.h"

using namespace haard;

FunctionExpression::FunctionExpression(Function* function) {
    set_kind(EXPR_FUNCTION);
    set_function(function);
}

FunctionExpression::~FunctionExpression() {
    delete function;
}

Function* FunctionExpression::get_function() {
    return function;
}

void FunctionExpression::set_function(Function* value) {
    function = value;
}
