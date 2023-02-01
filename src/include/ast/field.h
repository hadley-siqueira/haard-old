#ifndef HAARD_AST_FIELD_H
#define HAARD_AST_FIELD_H

#include "ast/type.h"
#include "ast/expression.h"

namespace haard {
    class Field {
        public:
            Field();
            virtual ~Field();

        public:
            Type* get_type() const;
            Expression* get_initial_value() const;

            void set_type(Type* value);
            void set_initial_value(Expression* value);
            void set_from_token(Token& token);

    private:
            int line;
            int column;
            std::string name;
            Type* type;
            Expression* initial_value;
    };
}

#endif
