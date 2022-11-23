#ifndef HAARD_AST_VARIABLE_H
#define HAARD_AST_VARIABLE_H

#include <string>
#include "token/token.h"
#include "ast/type.h"
#include "ast/identifier.h"

namespace haard {
    class Variable {
        public:
            Variable();
            Variable(Token& token);
            Variable(Identifier* id);
            ~Variable();

        public:
            int get_line();
            int get_column();
            const char* get_name();
            Type* get_type();
            int get_size_in_bytes();

            void set_line(int line);
            void set_column(int column);
            void set_name(const char* name);
            void set_type(Type* type);
            void set_from_token(Token& token);

            int get_uid();
            void set_uid(int uid);

            void set_kind(int kind);
            int get_kind();
            std::string get_cpp_name();

            Variable* clone();

        private:
            int uid;
            int line;
            int column;
            bool constant;
            int kind;
            const char* name;
            Type* type;
    };
}

#endif
