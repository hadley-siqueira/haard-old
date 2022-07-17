#ifndef HAARD_AST_EXPRESSION_H
#define HAARD_AST_EXPRESSION_H

#include "defs.h"
#include "ast/type.h"

namespace haard {
    class Expression {
        public:
            Expression();
            virtual ~Expression();

        public:
            int get_kind();
            Type* get_type();

            void set_kind(int kind);
            void set_type(Type* type);

        protected:
            int kind;
            Type* type;
    };
}

#endif
