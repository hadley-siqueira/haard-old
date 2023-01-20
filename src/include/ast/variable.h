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
            std::string get_name();
            Type* get_type();
            int get_size_in_bytes();

            void set_line(int line);
            void set_column(int column);
            void set_name(std::string name);
            void set_type(Type* type);
            void set_from_token(Token& token);

            int get_uid();
            void set_uid(int uid);

            void set_kind(int kind);
            int get_kind();
            std::string get_unique_name();

            Variable* clone();

            int get_offset() const;
            void set_offset(int value);

    private:
            int uid;
            int line;
            int column;
            int offset;
            bool constant;
            int kind;
            std::string name;
            Type* type;
    };
}

#endif
