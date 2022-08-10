#include <vector>
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
