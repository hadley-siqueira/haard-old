#include <vector>
#include <iostream>
#include <sstream>
#include "ast/type.h"
#include "ast/named_type.h"

using namespace haard;

class TypePool {
    public:
        void add(Type* type) {
            types.push_back(type);
        }

        ~TypePool() {
            for (int i = 0; i < types.size(); ++i) {
                delete types[i];
            }
        }

    private:
        std::vector<Type*> types;
};

TypePool type_pool;

Type::Type() {
    type_pool.add(this);
}

Type::Type(int kind) {
    this->kind = kind;
    type_pool.add(this);
}

Type::Type(int kind, Token& token) {
    this->kind = kind;
    line = token.get_line();
    column = token.get_column();
    type_pool.add(this);
}

Type::~Type() {

}

int Type::get_kind() {
    return kind;
}

int Type::get_line() {
    return line;
}

int Type::get_column() {
    return column;
}

std::string Type::get_qualified_name() {
    return to_str();
}

void Type::set_kind(int kind) {
    this->kind = kind;
}

void Type::set_line(int line) {
    this->line = line;
}

void Type::set_column(int column) {
    this->column = column;
}

bool Type::equal(Type* type) {
    return kind == type->kind;
}

std::string Type::to_str() {
    std::stringstream out;

    switch (kind) {
    case TYPE_U8:
        out << "u8";
        break;

    case TYPE_U16:
        out << "u16";
        break;

    case TYPE_U32:
        out << "u32";
        break;

    case TYPE_U64:
        out << "u64";
        break;

    case TYPE_I8:
        out << "i8";
        break;

    case TYPE_I16:
        out << "i16";
        break;

    case TYPE_I32:
        out << "i32";
        break;

    case TYPE_I64:
        out << "i64";
        break;

    case TYPE_VOID:
        out << "void";
        break;

    case TYPE_SYMBOL:
        out << "sym";
        break;

    case TYPE_BOOL:
        out << "bool";
        break;

    case TYPE_UCHAR:
        out << "uchar";
        break;

    case TYPE_CHAR:
        out << "char";
        break;

    case TYPE_USHORT:
        out << "ushort";
        break;

    case TYPE_SHORT:
        out << "short";
        break;

    case TYPE_UINT:
        out << "uint";
        break;

    case TYPE_INT:
        out << "int";
        break;

    case TYPE_ULONG:
        out << "ulong";
        break;

    case TYPE_LONG:
        out << "long";
        break;

    case TYPE_FLOAT:
        out << "float";
        break;

    case TYPE_DOUBLE:
        out << "double";
        break;

    case TYPE_STR:
        out << "str";
        break;

    default:
        break;
    }

    return out.str();
}

Type* Type::clone() {
    Type* other = new Type();

    other->kind = kind;
    other->line = line;
    other->column = column;

    return other;
}

bool Type::is_primitive() {
    return kind == TYPE_U8
        || kind == TYPE_U16
        || kind == TYPE_U32
        || kind == TYPE_U64
        || kind == TYPE_I8
        || kind == TYPE_I16
        || kind == TYPE_I32
        || kind == TYPE_I64
        || kind == TYPE_VOID
        || kind == TYPE_SYMBOL
        || kind == TYPE_BOOL
        || kind == TYPE_UCHAR
        || kind == TYPE_CHAR
        || kind == TYPE_USHORT
        || kind == TYPE_SHORT
        || kind == TYPE_UINT
        || kind == TYPE_INT
        || kind == TYPE_ULONG
        || kind == TYPE_LONG
        || kind == TYPE_FLOAT
        || kind == TYPE_DOUBLE;
}

Type* Type::promote(Type* other) {
    if (rank() > other->rank()) {
        return this;
    }

    return other;
}

int Type::rank() {
    switch (kind) {
    case TYPE_VOID:
        return 0;

    case TYPE_BOOL:
        return 1;

    case TYPE_I8:
    case TYPE_CHAR:
        return 2;

    case TYPE_U8:
    case TYPE_UCHAR:
        return 3;

    case TYPE_I16:
    case TYPE_SHORT:
        return 4;

    case TYPE_U16:
    case TYPE_USHORT:
        return 5;

    case TYPE_I32:
    case TYPE_INT:
        return 6;

    case TYPE_U32:
    case TYPE_UINT:
        return 7;

    case TYPE_LONG:
    case TYPE_I64:
        return 8;

    case TYPE_U64:
    case TYPE_ULONG:
        return 9;

    case TYPE_FLOAT:
        return 10;

    case TYPE_DOUBLE:
        return 11;

    case TYPE_SYMBOL:
    case TYPE_STR:
        return 12;
    }

    return 13;
}

int Type::get_size_in_bytes() {
    switch (kind) {
    case TYPE_I8:
    case TYPE_U8:
    case TYPE_CHAR:
    case TYPE_BOOL:
    case TYPE_UCHAR:
    case TYPE_VOID:
        return 1;

    case TYPE_I16:
    case TYPE_SHORT:
    case TYPE_U16:
    case TYPE_USHORT:
        return 2;

    case TYPE_I32:
    case TYPE_INT:
    case TYPE_U32:
    case TYPE_UINT:
    case TYPE_FLOAT:
        return 4;

    case TYPE_LONG:
    case TYPE_I64:
    case TYPE_U64:
    case TYPE_ULONG:
    case TYPE_DOUBLE:
        return 8;

    case TYPE_SYMBOL:
    case TYPE_STR:
        return ARCH_WORD_SIZE;
    }

    return 0;
}

int Type::get_alignment() {
    return get_size_in_bytes();
}

Scope* Type::get_scope() {
    return nullptr;
}

bool Type::is_signed() {
    switch (kind) {
    case TYPE_U8:
    case TYPE_UCHAR:
    case TYPE_U16:
    case TYPE_USHORT:
    case TYPE_U32:
    case TYPE_UINT:
    case TYPE_U64:
    case TYPE_ULONG:
        return false;
    }

    return true;
}

bool Type::is_class() {
    return false;
}

bool Type::is_integer_scalar() {
    switch (kind) {
    case TYPE_I8:
    case TYPE_U8:
    case TYPE_CHAR:
    case TYPE_BOOL:
    case TYPE_UCHAR:
    case TYPE_VOID:
    case TYPE_I16:
    case TYPE_SHORT:
    case TYPE_U16:
    case TYPE_USHORT:
    case TYPE_I32:
    case TYPE_INT:
    case TYPE_U32:
    case TYPE_UINT:
    case TYPE_LONG:
    case TYPE_I64:
    case TYPE_U64:
    case TYPE_ULONG:
    case TYPE_SYMBOL:
    case TYPE_STR:
        return true;
    }

    return false;
}

bool Type::is_user_type() {
    return kind == TYPE_NAMED;
}

bool Type::is_pointer() {
    return kind == TYPE_POINTER;
}
