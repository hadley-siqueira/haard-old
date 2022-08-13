#ifndef HAARD_AST_LITERAL_EXPRESSION_H
#define HAARD_AST_LITERAL_EXPRESSION_H

#include "token/token.h"
#include "ast/expression.h"

namespace haard {
    class Literal : public Expression {
        public:
            Literal(int kind);
            Literal(int kind, Token& token);

        public:
            const char* get_lexeme();
            Expression* clone();

        private:
            const char* lexeme;
            int line;
            int column;
    };
}

#endif
