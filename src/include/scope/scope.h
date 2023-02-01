#ifndef HAARD_SCOPE_H
#define HAARD_SCOPE_H

#include <map>
#include <vector>
#include "ast/ast.h"
#include "scope/symbol.h"
#include "ast/expression.h"

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

            Symbol* define_class(std::string& name, Class* klass);

            Symbol* define(Class* klass);
            Symbol* define(Function* func);
            Symbol* define(int kind, Variable* var);
            Symbol* define(Variable* var);
            Symbol* define_template(std::string name);

            bool has_parent();
            bool has_siblings();
            bool has_super();
            void add_sibling(Scope* scope);
            int siblings_count();

            Symbol* has(std::string name);
            Symbol* has_field(std::string name);
            Symbol* local_has(std::string name);
            Symbol* has_class(std::string name);

            std::vector<Variable*> get_variables_to_be_deleted();
            void add_deletable(Expression* expr);
            int deletables_count();
            Expression* get_deletable(int i);

            void debug();

            // new stuff
            Symbol* resolve(std::string& name);
            Symbol* resolve_local(std::string& name);
            Symbol* resolve_field(std::string& name);

        private:
            std::map<std::string, Symbol*> symbols;
            Scope* parent;
            Scope* super;
            std::vector<Scope*> siblings;
            std::vector<Expression*> deletables;

            // new stuff
            std::string qualified;
    };
}

#endif
