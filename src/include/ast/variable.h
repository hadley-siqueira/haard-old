#ifndef HAARD_AST_VARIABLE_H
#define HAARD_AST_VARIABLE_H

#include <string>
#include "token/token.h"
#include "ast/type.h"

namespace haard {
    class Variable {
        public:
            Variable();
            Variable(Token& token);
            ~Variable();

        public:
            int get_line();
            int get_column();
            std::string get_name();
            Type* get_type();

            void set_line(int line);
            void set_column(int column);
            void set_name(std::string name);
            void set_type(Type* type);
            void set_from_token(Token& token);

        private:
            int line;
            int column;
            bool constant;
            std::string name;
            Type* type;
    };
}

#endif
