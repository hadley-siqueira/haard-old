#include <iostream>
#include "scope/symbol.h"
#include "ast/variable.h"
#include "defs.h"
#include "printer/printer.h"

using namespace haard;

Symbol::Symbol() {
    name = nullptr;
}

Symbol::Symbol(std::string name) {
    this->name = name;
}

Symbol::Symbol(int kind, std::string name, SymbolDescriptor* descriptor) {
    this->kind = kind;
    this->name = name;
    add_descriptor(descriptor);
}

Symbol::~Symbol() {
    for (int i = 0; i < descriptors_count(); ++i) {
        delete descriptors[i];
    }
}

int Symbol::get_kind(int idx) {
    return descriptors[idx]->get_kind();
}

std::string Symbol::get_name() {
    return name;
}

SymbolDescriptor* Symbol::get_descriptor(int idx) {
    return descriptors[idx];
}

void Symbol::set_kind(int kind) {
    this->kind = kind;
}

void Symbol::set_name(std::string name) {
    this->name = name;
}

Type* Symbol::get_type(int idx) {
    return descriptors[idx]->get_type();
}

std::string Symbol::to_str(int idx) {
    std::stringstream ss;

    ss << name << ": " << get_descriptor(idx)->to_str();

    return ss.str();
}

std::string Symbol::get_qualified_name(int idx) {
    return descriptors[idx]->get_qualified_name();
}

Scope* Symbol::get_descriptor_scope(int idx) {
    return descriptors[idx]->get_descriptor_scope();
}

void Symbol::add_descriptor(SymbolDescriptor* descriptor) {
    descriptors.push_back(descriptor);
}

int Symbol::descriptors_count() {
    return descriptors.size();
}

int Symbol::get_size_in_bytes(int idx) {
    return descriptors[idx]->get_size_in_bytes();
}

int Symbol::get_alignment(int idx) {
    return descriptors[idx]->get_alignment();
}
