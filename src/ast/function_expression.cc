#include "ast/function_expression.h"

using namespace haard;

FunctionExpression::FunctionExpression(Function* function) {
    this->kind = EXPR_FUNCTION;
    this->function = function;
}

FunctionExpression::~FunctionExpression() {
    delete function;
}

Function* FunctionExpression::get_function() {
    return function;
}

Expression* FunctionExpression::clone() {
    return new FunctionExpression(function->clone());
}
