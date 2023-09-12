#ifndef HAARD_AST_FIELD_H
#define HAARD_AST_FIELD_H

#include "ast/type.h"
#include "ast/expression.h"
#include "ast/variable.h"

namespace haard {
    class Field : public Variable {
    public:
        Field();
        virtual ~Field();

    public:
        Expression* get_initial_value() const;
        void set_initial_value(Expression* value);

    private:
        Expression* initial_value;
    };
}

#endif
