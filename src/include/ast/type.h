#ifndef HAARD_AST_TYPE_H
#define HAARD_AST_TYPE_H

#include "token/token.h"

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

        protected:
            int kind;
            int line;
            int column;
    };
}

#endif
