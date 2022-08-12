#ifndef HAARD_SCOPE_H
#define HAARD_SCOPE_H

#include <map>
#include <vector>
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
            bool has_siblings();
            void add_sibling(Scope* scope);
            int siblings_count();

            Symbol* has(const char* name);
            Symbol* has_as_sibling(const char* name);
            Symbol* has_class(const char* name);

            void debug();

        private:
            std::map<const char*, Symbol*> symbols;
            Scope* parent;
            std::vector<Scope*> siblings;
    };
}

#endif
