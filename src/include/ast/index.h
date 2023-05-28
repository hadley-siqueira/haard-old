#ifndef HAARD_AST_INDEX_H
#define HAARD_AST_INDEX_H

#include "token/token.h"
#include "expression.h"

namespace haard {
    class Index : public Expression {
    public:
        Index(Expression* object=nullptr, Expression* index=nullptr);
        Index(Token& token, Expression* object=nullptr, Expression* index=nullptr);
        ~Index();

    public:
        std::string to_str();

        int get_line() const;
        void set_line(int value);

        int get_column() const;
        void set_column(int value);

        Expression* get_object() const;
        void set_object(Expression* value);

        Expression* get_index() const;
        void set_index(Expression* value);

    private:
        int line;
        int column;
        Expression* object;
        Expression* index;
    };
}

#endif
