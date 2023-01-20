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
            Scope* get_super();

            void set_parent(Scope* symtab);
            void set_super(Scope* symtab);

            Symbol* define(Class* klass);
            Symbol* define(Function* func);
            Symbol* define(int kind, Variable* var);
            Symbol* define(Variable* var);
            Symbol* define(TemplateType* type);

            bool has_parent();
            bool has_siblings();
            bool has_super();
            void add_sibling(Scope* scope);
            int siblings_count();

            Symbol* has(const char* name);
            Symbol* has_field(const char* name);
            Symbol* local_has(const char* name);
            Symbol* has_class(const char* name);

            void debug();

        private:
            std::map<std::string, Symbol*> symbols;
            Scope* parent;
            Scope* super;
            std::vector<Scope*> siblings;
    };
}

#endif
