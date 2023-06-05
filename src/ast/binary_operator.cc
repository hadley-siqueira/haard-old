#include "ast/binary_operator.h"

using namespace haard;

BinaryOperator::~BinaryOperator() {
    delete left;
    delete right;
}

Expression* BinaryOperator::get_left() const {
    return left;
}

void BinaryOperator::set_left(Expression* value) {
    left = value;
}

Expression* BinaryOperator::get_right() const {
    return right;
}

void BinaryOperator::set_right(Expression* value) {
    right = value;
}
