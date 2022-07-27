#include "ast/hash_type.h"

using namespace haard;

HashType::HashType(Token& token, Type* key, Type* value) {
    this->kind = TYPE_HASH;
    this->key = key;
    this->value = value;
    line = token.get_line();
    column = token.get_column();
}

HashType::~HashType() {
    delete key;
    delete value;
}

Type* HashType::get_key_type() {
    return key;
}

Type* HashType::get_value_type() {
    return value;
}
