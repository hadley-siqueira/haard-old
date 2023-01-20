#ifndef HAARD_AST_BINOP_H
#define HAARD_AST_BINOP_H

#include "token/token.h"
#include "ast/expression.h"
#include "scope/symbol.h"

namespace haard {
    class BinOp : public Expression {
        public:
            BinOp();
            BinOp(int kind, Expression* left=nullptr, Expression* right=nullptr);
            BinOp(int kind, Token& token, Expression* left=nullptr, Expression* right=nullptr);
            virtual ~BinOp();

        public:
            Expression* get_left();
            Expression* get_right();

            void set_left(Expression* left);
            void set_right(Expression* right);

            void set_initial_assign(bool value);
            bool get_initial_assign();

            Expression* clone();

            Symbol* get_constructor_call() const;
            void set_constructor_call(Symbol* value);

            int get_overloaded_index() const;
            void set_overloaded_index(int value);

    private:
            Expression* left;
            Expression* right;
            int line;
            int column;
            bool initial_assign;
            Symbol* constructor_call;
            int overloaded_index;
    };
}

#endif
