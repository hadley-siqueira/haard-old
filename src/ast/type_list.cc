#include "ast/type_list.h"

using namespace haard;

TypeList::TypeList(int kind, Token& token, Type* t1, Type* t2) {
    this->kind = kind;
    types.push_back(t1);
    types.push_back(t2);

    line = token.get_line();
    column = token.get_column();
}

TypeList::~TypeList() {
    for (int i = 0; i < types.size(); ++i) {
        delete types[i];
    }
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
