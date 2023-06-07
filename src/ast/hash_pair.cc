#include <sstream>
#include "ast/hash_pair.h"

using namespace haard;

HashPair::HashPair(Expression* left, Expression* right) {
    set_kind(EXPR_HASH_PAIR);
    set_left(left);
    set_right(right);
}

HashPair::HashPair(Token& token, Expression* left, Expression* right) {
    set_kind(EXPR_HASH_PAIR);
    set_left(left);
    set_right(right);
    set_from_token(token);
}
