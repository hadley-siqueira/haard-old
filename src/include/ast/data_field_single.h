#ifndef HAARD_AST_DATAFIELD_SINGLE_H
#define HAARD_AST_DATAFIELD_SINGLE_H

#include "ast/data_field.h"
#include "ast/expression.h"
#include "ast/type.h"

namespace haard {
    class DataFieldSingle : public DataField {
        public:
            Type* get_type() const;
            Expression* get_initial_value() const;

            void set_type(Type *value);
            void set_initial_value(Expression *value);

        private:
            Type* type;
            Expression* initial_value;
    };
}

#endif
