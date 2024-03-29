#ifndef HAARD_AST_CAST_EXPRESSION_H
#define HAARD_AST_CAST_EXPRESSION_H

#include "ast/expression.h"

namespace haard {
    class Cast : public Expression {
        public:
            Cast();
            Cast(Expression* expr, Type* type);
            ~Cast();

        public:
            Type* get_cast_type();
            Expression* get_expression();

            void set_cast_type(Type* type);
            void set_expression(Expression* expr);

        private:
            Type* cast_type;
            Expression* expression;
    };
}

#endif
