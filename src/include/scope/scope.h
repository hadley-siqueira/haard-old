#ifndef HAARD_SCOPE_H
#define HAARD_SCOPE_H

#include <map>
#include "ast/ast.h"
#include "scope/symbol.h"

namespace haard {
    class Scope {
        public:
            Scope();
            ~Scope();

        public:
            Scope* get_parent();

            void set_parent(Scope* symtab);

            Symbol* define(Class* klass);
            Symbol* define(Function* func);
            Symbol* define(int kind, Function* func);
            Symbol* define(int kind, Variable* var);

            bool has_parent();
            Symbol* has(const char* name);
            Symbol* has_class(const char* name);

            void debug();

        private:
            std::map<const char*, Symbol*> symbols;
            Scope* parent;
    };
}

#endif
