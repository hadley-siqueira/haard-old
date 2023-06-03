#include <sstream>
#include "ast/cast_expression.h"

using namespace haard;

CastExpression::CastExpression() {
    kind = EXPR_CAST;
    cast_type = nullptr;
    expression = nullptr;
}

CastExpression::CastExpression(Expression* expr, Type* type) {
    kind = EXPR_CAST;
    cast_type = type;
    expression = expr;
}

CastExpression::~CastExpression() {
    delete expression;
}

std::string CastExpression::to_str() {
    std::stringstream ss;

    ss << expression->to_str() << " as " << cast_type->to_str();

    return ss.str();
}

Type* CastExpression::get_cast_type() {
    return cast_type;
}

Expression *CastExpression::get_expression() {
    return expression;
}

void CastExpression::set_cast_type(Type* type) {
    cast_type = type;
}

void CastExpression::set_expression(Expression* expr) {
    expression = expr;
}

Expression* CastExpression::clone() {
    return nullptr;
}
