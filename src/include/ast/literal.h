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
            std::string get_lexeme();
            uint64_t to_u64();

            std::string to_str();

        private:
            std::string lexeme;
            int line;
            int column;
    };
}

#endif
