#include <sstream>
#include "ast/tuple_type.h"

using namespace haard;

TupleType::TupleType() {
    kind = TYPE_TUPLE;
    types = new TypeList();
}

TupleType::~TupleType() {
    delete types;
}

void TupleType::add_type(Type* type) {
    types->add_type(type);
}

Type* TupleType::get_type(int idx) {
    return types->get_type(idx);
}

int TupleType::types_count() {
    return types->types_count();
}

TypeList* TupleType::get_types() {
    return types;
}

bool TupleType::equal(Type* type) {
    if (type->get_kind() != kind) {
        return false;
    }

    TupleType* other = (TupleType*) type;
    return types->equal(other->get_types());
}

Type* TupleType::clone() {
    return nullptr;
}

std::string TupleType::to_str() {
    std::stringstream ss;

    ss << "(";
    ss << types->to_str();
    ss << ")";

    return ss.str();
}

std::string TupleType::get_qualified_name() {
    std::stringstream ss;

    ss << "(";
    ss << types->get_qualified_name();
    ss << ")";

    return ss.str();
}
