#ifndef HAARD_AST_SHIFT_RIGHT_ARITHMETIC_ASSIGNMENT_H
#define HAARD_AST_SHIFT_RIGHT_ARITHMETIC_ASSIGNMENT_H

#include "token/token.h"
#include "expression.h"

namespace haard {
    class ShiftRightArithmeticAssignment : public Expression {
    public:
        ShiftRightArithmeticAssignment(Expression* left=nullptr, Expression* right=nullptr);
        ShiftRightArithmeticAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
        ~ShiftRightArithmeticAssignment();

    public:
        std::string to_str();

        int get_line() const;
        void set_line(int value);

        int get_column() const;
        void set_column(int value);

        Expression* get_left() const;
        void set_left(Expression* value);

        Expression* get_right() const;
        void set_right(Expression* value);

    private:
        int line;
        int column;
        Expression* left;
        Expression* right;
    };
}

#endif
