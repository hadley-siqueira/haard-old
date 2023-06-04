#ifndef HAARD_AST_LOGICAL_NOT_OPER_H
#define HAARD_AST_LOGICAL_NOT_OPER_H

#include "token/token.h"
#include "expression.h"

namespace haard {
    class LogicalNotOper : public Expression {
    public:
        LogicalNotOper(Expression* expression=nullptr);
        LogicalNotOper(Token& token, Expression* expression=nullptr);
        ~LogicalNotOper();

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
