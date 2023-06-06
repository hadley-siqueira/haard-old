#ifndef HAARD_AST_THIS_H
#define HAARD_AST_THIS_H

#include "ast/expression.h"
#include "ast/expression_list.h"

namespace haard {
    class This : public Expression {
        public:
            This(Token& token);
            This();
            ~This();

        public:
            Expression* clone();

        private:
    };
}

#endif
