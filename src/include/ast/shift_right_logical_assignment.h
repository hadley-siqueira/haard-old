#ifndef HAARD_AST_SHIFT_RIGHT_LOGICAL_ASSIGNMENT_H
#define HAARD_AST_SHIFT_RIGHT_LOGICAL_ASSIGNMENT_H

#include "token/token.h"
#include "expression.h"

namespace haard {
    class ShiftRightLogicalAssignment : public Expression {
    public:
        ShiftRightLogicalAssignment(Expression* left=nullptr, Expression* right=nullptr);
        ShiftRightLogicalAssignment(Token& token, Expression* left=nullptr, Expression* right=nullptr);
        ~ShiftRightLogicalAssignment();

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
