#include <iostream>
#include <sstream>
#include "scope/scope.h"

using namespace haard;

Scope::Scope() {
    parent = nullptr;
    super = nullptr;
}

Scope::~Scope() {
    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
        delete it->second;
    }

    for (int i = 0; i < deletables.size(); ++i) {
        delete deletables[i];
    }
}

Scope* Scope::get_parent() {
    return parent;
}

Scope* Scope::get_super() {
    return super;
}

void Scope::set_parent(Scope* symtab) {
    parent = symtab;
}

void Scope::set_super(Scope* symtab) {
    super = symtab;
}

Symbol* Scope::define_class(Class* klass) {
    Symbol* sym;
    std::string name = klass->get_name();

    if (symbols.count(name) == 0) {
        sym = new Symbol(name);
        symbols[name] = sym;
    } else {
        sym = symbols[name];
    }

    SymbolDescriptor* desc = new SymbolDescriptor(SYM_CLASS, klass);
    sym->add_descriptor(desc);

    return sym;
}

Symbol* Scope::define_struct(std::string& name, Struct* obj) {
    Symbol* sym;// = new Symbol(SYM_STRUCT, name, obj);
    symbols[name] = sym;

    return sym;
}

Symbol* Scope::define_enum(std::string& name, Enum* obj) {
    Symbol* sym;// = new Symbol(SYM_ENUM, name, obj);
    symbols[name] = sym;

    return sym;
}

Symbol* Scope::define_union(std::string& name, Union* obj) {
    Symbol* sym;// = new Symbol(SYM_UNION, name, obj);
    symbols[name] = sym;

    return sym;
}

Symbol* Scope::define_type(int kind, std::string& name, CompoundTypeDescriptor* obj) {
    Symbol* sym;// = new Symbol(kind, name, obj);
    symbols[name] = sym;

    return sym;
}

Symbol* Scope::define_function(Function* obj) {
    Symbol* sym;
    std::string name = obj->get_name();

    if (symbols.count(name) == 0) {
        sym = new Symbol(name);
        symbols[name] = sym;
    } else {
        sym = symbols[name];
    }

    SymbolDescriptor* desc = new SymbolDescriptor(SYM_FUNCTION, obj);
    sym->add_descriptor(desc);

    return sym;
}

Symbol* Scope::define_template(NamedType* type) {
    Symbol* sym = new Symbol(type->get_name());
    symbols[type->get_name()] = sym;

    sym->add_descriptor(new SymbolDescriptor(SYM_TEMPLATE, nullptr));
    return sym;
}

Symbol* Scope::define_parameter(Variable* param) {
    std::string name = param->get_name();
    Symbol* sym = new Symbol(name);
    symbols[name] = sym;

    sym->add_descriptor(new SymbolDescriptor(SYM_PARAMETER, param));
    return sym;
}

Symbol *Scope::define_local_variable(Variable* obj) {
    Symbol* sym;// = new Symbol(SYM_VARIABLE, obj->get_name(), obj);
    symbols[obj->get_name()] = sym;

    return sym;
}

bool Scope::has_parent() {
    return parent != nullptr;
}

bool Scope::has_siblings() {
    return siblings.size() > 0;
}

bool Scope::has_super() {
    return super != nullptr;
}

void Scope::add_sibling(Scope* scope) {
    siblings.push_back(scope);
}

int Scope::siblings_count() {
    return siblings.size();
}

Symbol* Scope::local_has(std::string name) {
    if (symbols.count(name) > 0) {
        return symbols[name];
    }

    return nullptr;
}

Symbol* Scope::has_field(std::string name) {
    if (symbols.count(name) > 0) {
        return symbols[name];
    }

    if (has_super()) {
        return super->has_field(name);
    }

    return nullptr;
}

Symbol* Scope::has(std::string name) {
    Symbol* sym = nullptr;

    if (symbols.count(name) > 0) {
        return symbols[name];
    }

    sym = has_field(name);

    if (sym) {
        return sym;
    }

    if (has_parent()) {
        return parent->has(name);
    }

    if (has_siblings()) {
        for (int i = 0; i < siblings_count(); ++i) {
            sym = siblings[i]->local_has(name);

            if (sym != nullptr) {
                return sym;
            }
        }
    }

    return sym;
}

Symbol* Scope::has_class(std::string name) {
    Symbol* sym = nullptr;

    if (symbols.count(name) > 0) {
        sym = symbols[name];

        if (sym->get_kind() == SYM_CLASS) {
            return sym;
        }
    }

    if (has_parent()) {
        return parent->has_class(name);
    }

    return nullptr;
}

std::vector<Variable*> Scope::get_variables_to_be_deleted() {
    Variable* var;
    Type* type;
    int kind;
    std::vector<Variable*> vars;

    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
        Symbol* sym = it->second;

        switch (sym->get_kind()) {
        case SYM_CLASS_VARIABLE:
        case SYM_PARAMETER:
        case SYM_VARIABLE:
            var = (Variable*) sym->get_descriptor();
            type = var->get_type();

            if (type->get_kind() == TYPE_NAMED) {
                NamedType* named = (NamedType*) type;
                kind = named->get_symbol()->get_kind();

                if (kind == SYM_CLASS) {
                    vars.push_back(var);
                }
            }

        default:
            break;
        }
    }

    return vars;
}

void Scope::add_deletable(Expression* expr) {
    deletables.push_back(expr);
}

int Scope::deletables_count() {
    return deletables.size();
}

Expression* Scope::get_deletable(int i) {
    if (i < deletables_count()) {
        return deletables[i];
    }

    return nullptr;
}

std::string Scope::debug() {
    std::stringstream ss;
    Symbol* sym;

    if (has_parent()) {
        ss << parent->debug();
        ss << " -> ";
    }

    ss << "{\n";

    for (auto it = symbols.begin(); it != symbols.end(); ++it) {
        sym = it->second;
        ss << "    " << sym->to_str() << ",\n";
    }

    ss << "}\n";
    return ss.str();
}

Symbol* Scope::resolve(const std::string& name) {
    Symbol* sym = nullptr;

    sym = resolve_local(name);

    if (sym) {
        return sym;
    }

    sym = resolve_field(name);

    if (sym) {
        return sym;
    }

    if (has_parent()) {
        return parent->resolve(name);
    }

    if (has_siblings()) {
        for (int i = 0; i < siblings_count(); ++i) {
            sym = siblings[i]->resolve_local(name);

            if (sym != nullptr) {
                return sym;
            }
        }
    }

    return sym;
}

Symbol* Scope::resolve_local(const std::string& name) {
    if (symbols.count(name) > 0) {
        return symbols[name];
    }

    if (symbols.count(qualified + name) > 0) {
        return symbols[qualified + name];
    }

    return nullptr;
}

Symbol* Scope::resolve_field(const std::string& name) {
    if (symbols.count(name) > 0) {
        return symbols[name];
    }

    if (has_super()) {
        return super->has_field(name);
    }

    return nullptr;
}

std::string Scope::get_qualified() const
{
    return qualified;
}

void Scope::set_qualified(const std::string &value)
{
    qualified = value;
}
