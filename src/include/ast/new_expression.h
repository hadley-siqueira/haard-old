#ifndef HAARD_AST_NEW_H
#define HAARD_AST_NEW_H

#include "ast/expression.h"
#include "ast/expression_list.h"

namespace haard {
    class NewExpression : public Expression {
        public:
            NewExpression();
            ~NewExpression();

        public:
            Type* get_new_type();
            ExpressionList* get_arguments();
            Expression* get_array_size();

            void set_new_type(Type* type);
            void set_arguments(ExpressionList* args);
            void set_array_size(Expression* expr);

            bool has_arguments();

            Expression* clone();

        private:
            Type* new_type;
            ExpressionList* args;
            Expression* array_size;
    };
}

#endif
