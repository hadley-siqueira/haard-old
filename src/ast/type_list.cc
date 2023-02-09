#include <iostream>
#include <sstream>
#include "ast/type_list.h"

using namespace haard;

TypeList::TypeList() {

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

bool TypeList::equal(TypeList* other) {
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

TypeList* TypeList::clone() {
    TypeList* other = new TypeList();

    for (int i = 0; i < types.size(); ++i) {
        other->add_type(types[i]->clone());
    }

    return other;
}

std::string TypeList::get_qualified_name() {
    int i;
    std::stringstream ss;

    for (i = 0; i < types_count() - 1; ++i) {
        ss << get_type(i)->get_qualified_name() << ", ";
    }

    ss << get_type(i)->get_qualified_name();
    return ss.str();
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
