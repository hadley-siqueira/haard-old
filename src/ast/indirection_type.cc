#include "ast/indirection_type.h"

using namespace haard;

IndirectionType::IndirectionType(int kind, Token& token, Type* subtype) {
    this->kind = kind;
    this->subtype = subtype;

    line = token.get_line();
    column = token.get_column();
}

IndirectionType::~IndirectionType() {
    delete subtype;
}

Type* IndirectionType::get_subtype() {
    return subtype;
}
