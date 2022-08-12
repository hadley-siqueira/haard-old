#include <vector>
#include <sstream>
#include "ast/type.h"

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

std::string Type::to_cpp() {
    std::stringstream out;

    switch (kind) {
    case TYPE_U8:
        out << "uint8_t";
        break;

    case TYPE_U16:
        out << "uint16_t";
        break;

    case TYPE_U32:
        out << "uint32_t";
        break;

    case TYPE_U64:
        out << "uint64_t";
        break;

    case TYPE_I8:
        out << "int8_t";
        break;

    case TYPE_I16:
        out << "int16_t";
        break;

    case TYPE_I32:
        out << "int32_t";
        break;

    case TYPE_I64:
        out << "int64_t";
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
        out << "unsigned char";
        break;

    case TYPE_CHAR:
        out << "char";
        break;

    case TYPE_USHORT:
        out << "unsigned short";
        break;

    case TYPE_SHORT:
        out << "short";
        break;

    case TYPE_UINT:
        out << "unsigned int";
        break;

    case TYPE_INT:
        out << "int";
        break;

    case TYPE_ULONG:
        out << "unsigned long";
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
        out << "std::string";
        break;
    }

    return out.str();
}
