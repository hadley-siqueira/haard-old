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

Type* Symbol::get_type() {
    Variable* var = (Variable*) descriptors[0];
    Class* klass = (Class*) descriptors[0];

    switch (kind) {
    case SYM_CLASS:
        return klass->get_self_type();

    case SYM_FUNCTION:
        return nullptr;

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
        ss << "func";
        break;

    case SYM_PARAMETER:
        ss << "param";
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
            
std::string Symbol::to_cpp() {
    std::stringstream ss;

    // FIXME
    if (descriptors.size() == 0) {
        std::cout << __FILE__ << ' ' << __LINE__ << "Error: descriptor empty\n";
        exit(0);
    }

    Class* klass = (Class*) descriptors[0];
    Function* func = (Function*) descriptors[0];
    Variable* var = (Variable*) descriptors[0];

    switch (kind) {
    case SYM_CLASS:
        ss << "c" << klass->get_uid() << '_' << klass->get_name();
        break;

    case SYM_FUNCTION:
        ss << "f" << func->get_uid() << '_' << func->get_name();
        break;

    case SYM_PARAMETER:
        ss << "p" << var->get_uid() << '_' << var->get_name();
        break;

    case SYM_VARIABLE:
        ss << "v" << var->get_uid() << '_' << var->get_name();
        break;

    case SYM_CLASS_VARIABLE:
        ss << "cv" << var->get_uid() << '_' << var->get_name();
        break;
    }

    return ss.str();
}
