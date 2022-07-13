#ifndef HAARD_AST_FUNCTION_H
#define HAARD_AST_FUNCTION_H

#include <vector>
#include <string>
#include "token/token.h"
#include "ast/variable.h"

namespace haard {
    class Function {
        public:
            int get_line();
            int get_column();
            std::string get_name();
            Variable* get_parameter(int idx);

            void set_line(int line);
            void set_column(int column);
            void set_name(std::string name);
            void set_from_token(Token& token);

            int parameters_count();

            void add_parameter(Variable* param);

        private:
            int line;
            int column;
            std::string name;
            std::vector<Variable*> parameters;
            
    };
}

#endif
