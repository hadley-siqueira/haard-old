#ifndef HAARD_AST_TYPE_LIST_H
#define HAARD_AST_TYPE_LIST_H

#include <vector>
#include <string>
#include "token/token.h"
#include "ast/type.h"

namespace haard {
    class TypeList {
        public:
            TypeList();

        public:
            Type* get_type(int idx);
            int types_count();
            void add_type(Type* type);

            bool equal(TypeList* other);
            TypeList* clone();
            std::string get_qualified_name();
            std::string to_str();

        private:
            std::vector<Type*> types;
    };
}

#endif
