#include <iostream>
#include "ast/array_list_type.h"
#include "ast/literal.h"
#include "ast/indirection_type.h"

using namespace haard;

ArrayListType::ArrayListType() {

}

ArrayListType::ArrayListType(int kind, Token& token, Type* subtype, Expression* expr) {
    this->kind = kind;
    this->subtype = subtype;
    this->expression = expr;

    line = token.get_line();
    column = token.get_column();
}

ArrayListType::~ArrayListType() {
    delete expression;
}

Expression* ArrayListType::get_expression() {
    return expression;
}

Type* ArrayListType::get_subtype() {
    return subtype;
}

bool ArrayListType::equal(Type* type) {
    ArrayListType* other = (ArrayListType*) type;
    IndirectionType* ptype = (IndirectionType*) type;

    if (kind != type->get_kind() && type->get_kind() != TYPE_POINTER) {
        return false;
    }

    if (type->get_kind() == TYPE_POINTER) {
        if (!subtype->equal(ptype->get_subtype())) {
            return false;
        }
    } else {
        if (!subtype->equal(other->get_subtype())) {
            return false;
        }
    }

    if (expression && other->get_expression()) {

    }

    return true;
}

bool ArrayListType::is_primitive() {
    return false;
}

Type *ArrayListType::promote(Type *other) {
    return nullptr;
}

int ArrayListType::rank() {
    return 0;
}

int ArrayListType::get_size_in_bytes() {
    int size = subtype->get_size_in_bytes();

    if (expression->get_kind() == EXPR_LITERAL_INTEGER) {
        Literal* literal = (Literal*) expression;
        size = size * literal->to_u64();
    } else {
        std::cout << __FILE__ << "\nERROR\n";
        exit(0);
    }

    return size;
}

int ArrayListType::get_alignment() {
    return subtype->get_alignment();
}
