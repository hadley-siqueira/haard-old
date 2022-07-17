#ifndef HAARD_AST_IDENTIFIER_H
#define HAARD_AST_IDENTIFIER_H

#include <string>
#include "ast/expression.h"
#include "token/token.h"

namespace haard {
    class Identifier : public Expression {
        public:
            Identifier();
            Identifier(Token& token);

        public:
            int get_line();
            int get_column();
            std::string get_lexeme();

            void set_line(int line);
            void set_column(int column);
            void set_lexeme(std::string lexeme);
            
        private:
            int line;
            int column;
            std::string lexeme;
            
    };
}

#endif
