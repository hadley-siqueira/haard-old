#include "ast/type_list.h"

using namespace haard;

TypeList::TypeList(int kind, Token& token, Type* t1, Type* t2) {
    this->kind = kind;

    if (t1 != nullptr) {
        types.push_back(t1);
    }

    if (t2 != nullptr) {
        types.push_back(t2);
    }

    line = token.get_line();
    column = token.get_column();
}

TypeList::~TypeList() {
    
}

Type* TypeList::get_type(int idx) {
    if (idx < types.size()) {
        return types[idx];
    }

    return nullptr;
}

int TypeList::types_count() {
    return types.size();
}

void TypeList::add_type(Type* type) {
    types.push_back(type);
}
