#ifndef HAARD_SCOPE_H
#define HAARD_SCOPE_H

#include <map>
#include <vector>
#include "ast/ast.h"
#include "scope/symbol.h"
#include "ast/expression.h"

namespace haard {
    class Statement;

    class Scope {
        public:
            Scope();
            ~Scope();

        public:
            Scope* get_parent();
            Scope* get_super();

            void set_parent(Scope* symtab);
            void set_super(Scope* symtab);

            SymbolDescriptor* define(int kind, std::string name, void* obj);
            SymbolDescriptor* define_class(Class* klass);
            SymbolDescriptor* define_struct(Struct* obj);
            SymbolDescriptor* define_enum(Enum* obj);
            SymbolDescriptor* define_union(std::string& name, Union* obj);
            SymbolDescriptor* define_function(Function* obj);
            SymbolDescriptor* define_method(Function* obj);
            SymbolDescriptor* define_template(NamedType* type);
            SymbolDescriptor* define_parameter(Variable* param);
            SymbolDescriptor* define_local_variable(Variable* obj);

            bool has_parent();
            bool has_siblings();
            bool has_super();
            void add_sibling(Scope* scope);
            int siblings_count();

            std::vector<Variable*> get_variables_to_be_deleted();
            void add_deletable(Expression* expr);
            int deletables_count();
            Expression* get_deletable(int i);

            std::string debug();

            // new stuff
            Symbol* resolve(const std::string& name);
            Symbol* resolve_local(const std::string& name);
            Symbol* resolve_field(const std::string& name);

            std::string get_qualified() const;
            void set_qualified(const std::string& value);

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
