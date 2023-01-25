#ifndef HAARD_AST_IDENTIFIER_H
#define HAARD_AST_IDENTIFIER_H

#include <string>
#include "ast/expression.h"
#include "token/token.h"
#include "scope/symbol.h"

namespace haard {
    class Identifier : public Expression {
        public:
            Identifier();
            Identifier(Token& token);
            Identifier(Token& alias, Token& name);
            Identifier(std::string name);

        public:
            int get_line();
            int get_column();
            std::string get_name();
            Symbol* get_symbol();

            void set_line(int line);
            void set_column(int column);
            void set_name(std::string name);
            void set_symbol(Symbol* symbol);

            int get_overloaded_index();
            void set_overloaded_index(int idx);

            std::string get_unique_name();

            Expression* clone();

            bool is_local_variable();
            bool is_class_variable();
            
            bool has_scope();
            bool has_global_scope();
            std::string get_alias() const;
            void set_alias(const std::string& value);

    private:
            int line;
            int column;
            std::string name;
            std::string alias;
            Symbol* symbol;
            int overloaded_index;
            
    };
}

#endif
