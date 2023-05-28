#ifndef HAARD_AST_DOT_H
#define HAARD_AST_DOT_H

#include "token/token.h"
#include "expression.h"

namespace haard {
    class Dot : public Expression {
    public:
        Dot(Expression* object=nullptr, Expression* field=nullptr);
        Dot(Token& token, Expression* object=nullptr, Expression* field=nullptr);
        ~Dot();

    public:
        std::string to_str();

        int get_line() const;
        void set_line(int value);

        int get_column() const;
        void set_column(int value);

        Expression* get_object() const;
        void set_object(Expression* value);

        Expression* get_field() const;
        void set_field(Expression* value);

    private:
        int line;
        int column;
        Expression* object;
        Expression* field;
    };
}

#endif
