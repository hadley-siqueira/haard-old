#include <sstream>
#include "ast/cast.h"

using namespace haard;

Cast::Cast() {
    set_kind(EXPR_CAST);
    cast_type = nullptr;
    expression = nullptr;
}

Cast::Cast(Expression* expr, Type* type) {
    set_kind(EXPR_CAST);
    cast_type = type;
    expression = expr;
}

Cast::~Cast() {
    delete expression;
}

Type* Cast::get_cast_type() {
    return cast_type;
}

Expression *Cast::get_expression() {
    return expression;
}

void Cast::set_cast_type(Type* type) {
    cast_type = type;
}

void Cast::set_expression(Expression* expr) {
    expression = expr;
}
