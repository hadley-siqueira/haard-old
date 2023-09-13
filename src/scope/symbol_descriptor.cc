#include <iostream>

#include "defs.h"
#include "ast/ast.h"
#include "printer/printer.h"
#include "scope/symbol_descriptor.h"

using namespace haard;

SymbolDescriptor::SymbolDescriptor() {
    descriptor = nullptr;
}

SymbolDescriptor::SymbolDescriptor(int kind, void* descriptor) {
    set_kind(kind);
    set_descriptor(descriptor);
}

int SymbolDescriptor::get_kind() const {
    return kind;
}

void SymbolDescriptor::set_kind(int value) {
    kind = value;
}

void* SymbolDescriptor::get_descriptor() const {
    return descriptor;
}

void SymbolDescriptor::set_descriptor(void* value) {
    descriptor = value;
}

Type* SymbolDescriptor::get_type() {
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

std::string SymbolDescriptor::to_str() {
    std::stringstream ss;

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
        //ss << get_type()->to_str();
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
        ss << "cvar(";

        if (get_type()) {
            Printer p;
            p.print_type(get_type());
            ss << p.to_str();
        }

        ss << ")";
        break;
    }

    ss << ">";
    return ss.str();
}

std::string SymbolDescriptor::get_qualified_name() {
    std::stringstream ss;

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
        ss << type->get_name();
        break;
    }

    return ss.str();
}

int SymbolDescriptor::get_size_in_bytes() {
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

int SymbolDescriptor::get_alignment() {
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

Scope* SymbolDescriptor::get_descriptor_scope() {
    Class* klass = (Class*) descriptor;

    switch (kind) {
    case SYM_CLASS:
        return klass->get_scope();
    }

    return nullptr;
}

Symbol* SymbolDescriptor::get_symbol() const {
    return symbol;
}

void SymbolDescriptor::set_symbol(Symbol* value) {
    symbol = value;
}
