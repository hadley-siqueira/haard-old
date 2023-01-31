#include <iostream>
#include <sstream>
#include "ast/type_list.h"

using namespace haard;

TypeList::TypeList() {

}

TypeList::TypeList(int kind) {
    this->kind = kind;
}

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

bool TypeList::equal(Type* type) {
    TypeList* other = (TypeList*) type;

    if (kind != type->get_kind()) {
        return false;
    }

    if (types_count() != other->types_count()) {
        return false;
    }
 
    for (int i = 0; i < types.size(); ++i) {
        if (!types[i]->equal(other->get_type(i))) {
            return false;
        }
    }

    return true;
}

bool TypeList::types_equal(TypeList* other) {
    if (types_count() != other->types_count()) {
        return false;
    }

    for (int i = 0; i < types.size(); ++i) {
        if (!types[i]->equal(other->get_type(i))) {
            return false;
        }
    }

    return true;
}

bool TypeList::check_arguments_type(TypeList* args) {
    if (args->types_count() != types_count() - 1) {
        return false;
    }

    for (int i = 0; i < types.size() - 1; ++i) {
        if (!args->get_type(i)->equal(types[i])) {
            return false;
        }
    }

    return true;
}

Type* TypeList::get_return_type() {
    if (types.size() > 0) {
        return types[types.size() - 1];
    }

    return nullptr;
}

Type* TypeList::clone() {
    TypeList* other = new TypeList();

    other->kind = kind;
    for (int i = 0; i < types.size(); ++i) {
        other->add_type(types[i]->clone());
    }

    return other;
}

std::string TypeList::to_str() {
    int i;
    std::stringstream ss;

    if (types_count() > 0) {
        for (i = 0; i < types.size() - 1; ++i) {
            ss << types[i]->to_str() << ", ";
        }

        ss << types[i]->to_str();
    }

    return ss.str();
}
