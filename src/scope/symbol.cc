#include <iostream>
#include "scope/symbol.h"
#include "ast/variable.h"
#include "defs.h"
#include "printer/printer.h"

using namespace haard;

Symbol::Symbol() {
    name = nullptr;
}

Symbol::Symbol(int kind, const char* name, void* descriptor) {
    this->kind = kind;
    this->name = name;
    descriptors.push_back(descriptor);
}

int Symbol::get_kind() {
    return kind;
}

const char* Symbol::get_name() {
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

void Symbol::set_name(const char* name) {
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

std::string Symbol::to_str() {
    std::stringstream ss;

    ss << "<" << name << ":";

    switch (kind) {
    case SYM_CLASS:
        ss << "class";
        break;

    case SYM_FUNCTION:
        ss << "func(";

        if (get_type()) {
            Printer p;
            p.print_type(get_type());
            ss << p.to_str();
        }

        ss << ")";
        break;

    case SYM_METHOD:
        ss << "method(";

        if (get_type()) {
            Printer p;
            p.print_type(get_type());
            ss << p.to_str();
        }

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

    case SYM_TEMPLATE:
        ss << "template";
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
        ss << var->get_cpp_name();
        break;
    }

    return ss.str();
}

int Symbol::overloaded_count() {
    return descriptors.size();
}

void* Symbol::get_overloaded(TypeList* types) {
    return nullptr;
}
