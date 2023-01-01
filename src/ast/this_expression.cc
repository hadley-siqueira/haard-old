#include "ast/this_expression.h"

using namespace haard;

haard::ThisExpression::ThisExpression(haard::Token& token) {
    this->kind = EXPR_THIS;
}

ThisExpression::ThisExpression() {
    this->kind = EXPR_THIS;
}

ThisExpression::~ThisExpression() {

}

Expression* ThisExpression::clone() {
    return nullptr;
}
