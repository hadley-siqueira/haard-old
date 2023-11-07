#ifndef HAARD_AST_TYPE_H
#define HAARD_AST_TYPE_H

#include "token/token.h"
#include "defs.h"

namespace haard {
    class Scope;
    class Symbol;

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
            virtual std::string get_qualified_name();

            void set_kind(int kind);
            void set_line(int line);
            void set_column(int column);

            virtual bool equal(Type* type);
            virtual std::string to_str();
            virtual Type* clone();
            virtual bool is_primitive();
            virtual Type* promote(Type* other);
            virtual int rank();
            virtual int get_size_in_bytes();
            virtual int get_alignment();
            virtual Scope* get_scope();
            virtual bool is_signed();
            virtual bool is_class();
            virtual bool is_integer_scalar();
            virtual bool is_user_type();
            bool is_pointer();

        protected:
            int kind;
            int line;
            int column;
    };
}

#endif
