#ifndef HAARD_AST_UNARY_PLUS_H
#define HAARD_AST_UNARY_PLUS_H

#include "token/token.h"
#include "expression.h"

namespace haard {
    class UnaryPlus : public Expression {
    public:
        UnaryPlus(Expression* expression=nullptr);
        UnaryPlus(Token& token, Expression* expression=nullptr);
        ~UnaryPlus();

    public:
        std::string to_str();

        int get_line() const;
        void set_line(int value);

        int get_column() const;
        void set_column(int value);

        Expression* get_expression() const;
        void set_expression(Expression* expression);

    private:
        int line;
        int column;
        Expression* expression;
    };
}

#endif
