#include <iostream>
#include "scope/symbol.h"
#include "ast/variable.h"
#include "defs.h"
#include "printer/printer.h"

using namespace haard;

Symbol::Symbol() {
    name = nullptr;
}

Symbol::Symbol(int kind, std::string name, void* descriptor) {
    this->kind = kind;
    this->name = name;
    descriptors.push_back(descriptor);
}

int Symbol::get_kind() {
    return kind;
}

std::string Symbol::get_name() {
    return name;
}

void* Symbol::get_descriptor(int idx) {
    if (idx < descriptors.size()) {
        return descriptors[idx];
    }

    return nullptr;
}

void Symbol::set_kind(int kind) {
    this->kind = kind;
}

void Symbol::set_name(std::string name) {
    this->name = name;
}

void Symbol::add_descriptor(void* descriptor) {
    descriptors.push_back(descriptor);
}

Type* Symbol::get_type(int idx) {
    Variable* var = (Variable*) descriptors[idx];
    Class* klass = (Class*) descriptors[idx];
    Function* func = (Function*) descriptors[idx];

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

std::string Symbol::to_str(int idx) {
    std::stringstream ss;
    NamedType* named = (NamedType*) descriptors[idx];

    ss << "<" << name << ":";

    switch (kind) {
    case SYM_CLASS:
        ss << "class";
        break;

    case SYM_FUNCTION:
        ss << "func(";
        ss << get_type(idx)->to_str();
        ss << ")";
        break;

    case SYM_METHOD:
        ss << "method(";
        ss << get_type(idx)->to_str();
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
            
std::string Symbol::to_cpp(int idx) {
    std::stringstream ss;

    // FIXME
    if (descriptors.size() == 0) {
        std::cout << __FILE__ << ' ' << __LINE__ << "Error: descriptor empty\n";
        exit(0);
    }

    Class* klass = (Class*) descriptors[idx];
    Function* func = (Function*) descriptors[idx];
    Variable* var = (Variable*) descriptors[idx];

    switch (kind) {
    case SYM_CLASS:
        ss << klass->get_cpp_name();
        break;

    case SYM_FUNCTION:
    case SYM_METHOD:
        ss << func->get_cpp_name();
        break;

    case SYM_PARAMETER:
    case SYM_VARIABLE:
    case SYM_CLASS_VARIABLE:
        ss << var->get_unique_name();
        break;
    }

    return ss.str();
}

std::string Symbol::get_qualified_name(int idx) {
    std::stringstream ss;

    // FIXME
    if (descriptors.size() == 0) {
        std::cout << __FILE__ << ' ' << __LINE__ << "Error: descriptor empty\n";
        exit(0);
    }

    Class* klass = (Class*) descriptors[idx];
    Function* func = (Function*) descriptors[idx];
    Variable* var = (Variable*) descriptors[idx];
    NamedType* type = (NamedType*) descriptors[idx];

    switch (kind) {
    case SYM_CLASS:
        ss << klass->get_qualified_name();
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
    }

    return ss.str();
}

int Symbol::overloaded_count() {
    return descriptors.size();
}

int Symbol::get_overloaded(TypeList* types) {
    Function* f;
    FunctionType* ft;
    int i;
    bool found = false;

    // try
    for (i = 0; i < overloaded_count(); ++i) {
        f = (Function*) get_descriptor(i);
        ft = (FunctionType*) f->get_self_type();

        if (ft->check_arguments_type(types)) {
            found = true;
            break;
        }
    }

    if (!found) {
        for (i = 0; i < overloaded_count(); ++i) {
            f = (Function*) get_descriptor(i);
            ft = (FunctionType*) f->get_self_type();

            if (ft->check_arguments_type_with_conversion(types)) {
                found = true;
                break;
            }
        }
    }

    if (found) {
        return i;
    }

    return -1;
}

int Symbol::get_overloaded(void* ptr) {
    for (int i = 0; i < overloaded_count(); ++i) {
        if (ptr == get_descriptor(i)) {
            return i;
        }
    }

    return -1;
}

Scope* Symbol::get_descriptor_scope(int idx) {
    Class* klass = (Class*) descriptors[idx];

    switch (kind) {
    case SYM_CLASS:
        return klass->get_scope();
    }

    return nullptr;
}

int Symbol::get_size_in_bytes(int idx) {
    Class* klass = (Class*) descriptors[idx];
    Variable* var = (Variable*) descriptors[idx];
    NamedType* tt = (NamedType*) descriptors[idx];

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

int Symbol::get_alignment(int idx) {
    Class* klass = (Class*) descriptors[idx];
    Function* func = (Function*) descriptors[idx];
    Variable* var = (Variable*) descriptors[idx];
    NamedType* tt = (NamedType*) descriptors[idx];

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
