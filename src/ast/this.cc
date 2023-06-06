#include "ast/this.h"

using namespace haard;

haard::This::This(haard::Token& token) {
    set_kind(EXPR_THIS);
    set_from_token(token);
}

This::This() {
    set_kind(EXPR_THIS);
}

This::~This() {

}

Expression* This::clone() {
    return nullptr;
}
