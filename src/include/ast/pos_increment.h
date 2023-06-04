#ifndef HAARD_AST_POS_INCREMENT_H
#define HAARD_AST_POS_INCREMENT_H

#include "token/token.h"
#include "expression.h"

namespace haard {
    class PosIncrement : public Expression {
    public:
        PosIncrement(Expression* expression=nullptr);
        PosIncrement(Token& token, Expression* expression=nullptr);
        ~PosIncrement();

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
