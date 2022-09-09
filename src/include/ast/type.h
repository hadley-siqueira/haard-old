#ifndef HAARD_AST_TYPE_H
#define HAARD_AST_TYPE_H

#include "token/token.h"
#include "defs.h"

namespace haard {
    class Type {
        public:
            Type();
            Type(int kind);
            Type(int kind, Token& token);
            virtual ~Type();

        public:
            int get_kind();
            int get_line();
            int get_column();

            void set_kind(int kind);
            void set_line(int line);
            void set_column(int column);

            virtual bool equal(Type* type);
            virtual std::string to_cpp();
            virtual std::string to_str();
            virtual Type* clone();
            virtual bool is_primitive();
            virtual Type* promote(Type* other);
            virtual int rank();

        protected:
            int kind;
            int line;
            int column;
    };
}

#endif
