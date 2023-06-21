#include <iostream>
#include "scope/symbol.h"
#include "ast/variable.h"
#include "defs.h"
#include "printer/printer.h"

using namespace haard;

Symbol::Symbol() {
    name = nullptr;
    descriptor = nullptr;
}

Symbol::Symbol(int kind, std::string name, void* descriptor) {
    this->kind = kind;
    this->name = name;
    this->descriptor = descriptor;
}

int Symbol::get_kind() {
    return kind;
}

std::string Symbol::get_name() {
    return name;
}

void* Symbol::get_descriptor() {
    return descriptor;
}

void Symbol::set_kind(int kind) {
    this->kind = kind;
}

void Symbol::set_name(std::string name) {
    this->name = name;
}

Type* Symbol::get_type() {
    Variable* var = (Variable*) descriptor;
    Class* klass = (Class*) descriptor;
    Function* func = (Function*) descriptor;

    switch (kind) {
    case SYM_CLASS:
        return klass->get_self_type();

    case SYM_FUNCTION:
        return func->get_self_type();

    case SYM_METHOD:
        return func->get_self_type();

    case SYM_PARAMETER:
    case SYM_VARIABLE:
    case SYM_CLASS_VARIABLE:
        return var->get_type();
    }

    return nullptr;
}

std::string Symbol::to_str() {
    std::stringstream ss;
    NamedType* named = (NamedType*) descriptor;

    ss << "<" << name << ":";

    switch (kind) {
    case SYM_CLASS:
        ss << "class";
        break;

    case SYM_STRUCT:
        ss << "struct";
        break;

    case SYM_UNION:
        ss << "union";
        break;

    case SYM_ENUM:
        ss << "enum";
        break;

    case SYM_FUNCTION:
        ss << "func(";
        //ss << get_type(idx)->to_str();
        ss << ")";
        break;

    case SYM_METHOD:
        ss << "method(";
        ss << get_type()->to_str();
        ss << ")";
        break;

    case SYM_PARAMETER:
        ss << "param(";

        if (get_type()) {
            Printer p;
            p.print_type(get_type());
            ss << p.to_str();
        }

        ss << ")";
        break;

    case SYM_VARIABLE:
        ss << "var(";

        if (get_type()) {
            Printer p;
            p.print_type(get_type());
            ss << p.to_str();
        }

        ss << ")";
        break;

    case SYM_CLASS_VARIABLE:
        ss << "cvar";
        break;
    }

    ss << ">";
    return ss.str();
}

std::string Symbol::get_qualified_name() {
    std::stringstream ss;

    Class* klass = (Class*) descriptor;
    Function* func = (Function*) descriptor;
    Variable* var = (Variable*) descriptor;
    NamedType* type = (NamedType*) descriptor;
    CompoundTypeDescriptor* type_decl = (CompoundTypeDescriptor*) descriptor;

    switch (kind) {
    case SYM_CLASS:
    case SYM_ENUM:
    case SYM_STRUCT:
    case SYM_UNION:
        ss << type_decl->get_qualified_name();
        break;

    case SYM_FUNCTION:
    case SYM_METHOD:
        ss << func->get_qualified_name();
        break;

    case SYM_PARAMETER:
    case SYM_VARIABLE:
    case SYM_CLASS_VARIABLE:
        ss << var->get_unique_name();
        break;

    case SYM_TEMPLATE:
        ss << "%" << ((long long) get_descriptor());
        break;
    }

    return ss.str();
}

Scope* Symbol::get_descriptor_scope(int idx) {
    Class* klass = (Class*) descriptor;

    switch (kind) {
    case SYM_CLASS:
        return klass->get_scope();
    }

    return nullptr;
}

bool Symbol::has_template(TemplateHeader* types) {
    for (int i = 0; i < templates.size(); ++i) {
        if (templates[i]->equal(types->get_types())) {
            return true;
        }
    }

    return false;
}

void Symbol::add_template(TypeList* types) {
    templates.push_back(types);
}

int Symbol::get_size_in_bytes() {
    Class* klass = (Class*) descriptor;
    Variable* var = (Variable*) descriptor;
    NamedType* tt = (NamedType*) descriptor;

    switch (kind) {
    case SYM_CLASS:
        return klass->get_size_in_bytes();

    case SYM_FUNCTION:
    case SYM_METHOD:
        return ARCH_WORD_SIZE;

    case SYM_PARAMETER:
    case SYM_VARIABLE:
    case SYM_CLASS_VARIABLE:
        return var->get_size_in_bytes();
    }

    return 1;
}

int Symbol::get_alignment() {
    Class* klass = (Class*) descriptor;
    Function* func = (Function*) descriptor;
    Variable* var = (Variable*) descriptor;
    NamedType* tt = (NamedType*) descriptor;

    switch (kind) {
    case SYM_CLASS:
        return klass->get_alignment();

    case SYM_FUNCTION:
    case SYM_METHOD:
        return ARCH_WORD_SIZE;

    case SYM_PARAMETER:
    case SYM_VARIABLE:
    case SYM_CLASS_VARIABLE:
        return var->get_type()->get_alignment();

    }

    return ARCH_WORD_SIZE;
}
