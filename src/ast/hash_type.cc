#include "ast/hash_type.h"

using namespace haard;

HashType::HashType() {
    this->kind = TYPE_HASH;
    key = nullptr;
    value = nullptr;
}

HashType::HashType(Token& token, Type* key, Type* value) {
    this->kind = TYPE_HASH;
    this->key = key;
    this->value = value;
    line = token.get_line();
    column = token.get_column();
}

HashType::~HashType() {
    // delete key;
    // delete value;
}

Type* HashType::get_key_type() {
    return key;
}

Type* HashType::get_value_type() {
    return value;
}

Type* HashType::clone() {
    HashType* other = new HashType();

    if (key) {
        other->key = key->clone();
    }

    if (value) {
        other->value = value->clone();
    }

    return other;
}
