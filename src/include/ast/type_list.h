#ifndef HAARD_AST_TYPE_LIST_H
#define HAARD_AST_TYPE_LIST_H

#include <vector>
#include "token/token.h"
#include "ast/type.h"

namespace haard {
    class TypeList : public Type {
        public:
            TypeList(int kind, Token& token, Type* t1=nullptr, Type* t2=nullptr);
            ~TypeList();

        public:
            Type* get_type(int idx);
            int types_count();
            void add_type(Type* type);

        private:
            std::vector<Type*> types;
    };
}

#endif
