#ifndef HAARD_AST_CAST_EXPRESSION_H
#define HAARD_AST_CAST_EXPRESSION_H

#include "ast/expression.h"

namespace haard {
    class CastExpression : public Expression {
        public:
            CastExpression();
            CastExpression(Expression* expr, Type* type);
            ~CastExpression();

        public:
            Type* get_cast_type();
            Expression* get_expression();

            void set_cast_type(Type* type);
            void set_expression(Expression* expr);

            Expression* clone();

        private:
            Type* cast_type;
            Expression* expression;
    };
}

#endif
