#ifndef HAARD_AST_LITERAL_EXPRESSION_H
#define HAARD_AST_LITERAL_EXPRESSION_H

#include "token/token.h"
#include "ast/expression.h"

namespace haard {
    class Literal : public Expression {
        public:
            Literal(int kind, Token& token);

        public:
            std::string get_lexeme();

        private:
            std::string lexeme;
            int line;
            int column;
    };
}

#endif
