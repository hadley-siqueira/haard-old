#ifndef HAARD_AST_LITERAL_EXPRESSION_H
#define HAARD_AST_LITERAL_EXPRESSION_H

#include "token/token.h"
#include "ast/expression.h"

namespace haard {
    class Literal : public Expression {
        public:
            Literal(int kind);
            Literal(int kind, Token& token);
            Literal(int kind, std::string str);

        public:
            const char* get_lexeme();
            Expression* clone();
            uint64_t to_u64();

        private:
            const char* lexeme;
            int line;
            int column;
    };
}

#endif
