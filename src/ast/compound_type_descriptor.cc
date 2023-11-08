#include <iostream>
#include <sstream>
#include "ast/module.h"
#include "scope/scope.h"
#include "ast/compound_type_descriptor.h"

using namespace haard;

CompoundTypeDescriptor::CompoundTypeDescriptor() {
    self_type = nullptr;
    super_type = nullptr;
    destructor = nullptr;
    scope = new Scope();
    size_in_bytes = -1;
}

CompoundTypeDescriptor::~CompoundTypeDescriptor() {
    for (int i = 0; i < methods.size(); ++i) {
        delete methods[i];
    }

    for (int i = 0; i < fields.size(); ++i) {
        delete fields[i];
    }

    delete scope;
}

std::vector<Annotation*> CompoundTypeDescriptor::get_annotations() const {
    return annotations;
}

std::string CompoundTypeDescriptor::get_qualified_name() {
    int i;
    std::stringstream ss;

    ss << get_module()->get_relative_path() << "." << get_name();

    if (get_template_header()) {
        ss << get_template_header()->get_qualified_name();
    }

    return ss.str();
}

int CompoundTypeDescriptor::get_size_in_bytes() {
    return size_in_bytes;
}

int CompoundTypeDescriptor::get_alignment() {
    return alignment;
}

void CompoundTypeDescriptor::set_alignment(int value) {
    alignment = value;
}

void CompoundTypeDescriptor::set_annotations(const std::vector<Annotation*>& value) {
    annotations = value;
}

int CompoundTypeDescriptor::methods_count() {
    return methods.size();
}

int CompoundTypeDescriptor::constructors_count() {
    return constructors.size();
}

int CompoundTypeDescriptor::fields_count() {
    return fields.size();
}

void CompoundTypeDescriptor::add_field(Variable* field) {
    fields.push_back(field);
}

void CompoundTypeDescriptor::add_method(Function* method) {
    methods.push_back(method);

    if (method->get_name() == "init") {
        constructors.push_back(method);
    }

    if (method->get_name() == "destroy") {
        destructor = method;
    }

    method->set_compound(this);
    method->set_module(get_module());
    method->get_scope()->set_parent(get_scope());
}

Scope* CompoundTypeDescriptor::get_scope() const {
    return scope;
}

std::string CompoundTypeDescriptor::get_full_filepath() {
    return get_module()->get_path();
}

std::string CompoundTypeDescriptor::get_relative_filepath() {
    return get_module()->get_relative_path();
}

Variable* CompoundTypeDescriptor::get_field(int idx) {
    if (idx < fields_count()) {
        return fields[idx];
    }

    return nullptr;
}

Function* CompoundTypeDescriptor::get_method(int idx) {
    if (idx < methods_count()) {
        return methods[idx];
    }

    return nullptr;
}

Function* CompoundTypeDescriptor::get_destructor() {
    return destructor;
}

void CompoundTypeDescriptor::set_destructor(Function* value) {
    destructor = value;
}

void CompoundTypeDescriptor::set_scope(Scope *value) {
    scope = value;
}

Type* CompoundTypeDescriptor::get_super_type() const {
    return super_type;
}

void CompoundTypeDescriptor::set_super_type(Type* value) {
    super_type = value;
}

NamedType* CompoundTypeDescriptor::get_self_type() {
    return self_type;
}

void CompoundTypeDescriptor::set_self_type(NamedType* type) {
    self_type = type;
}

CompoundTypeDescriptor* CompoundTypeDescriptor::get_super_descriptor() {
    CompoundTypeDescriptor* super = nullptr;

    if (get_super_type()) {
        super = (CompoundTypeDescriptor*) ((NamedType*) super_type)->get_symbol_descriptor()->get_descriptor();
    }

    return super;
}

void CompoundTypeDescriptor::set_from_token(Token& token) {
    set_name(token.get_lexeme());
    set_line(token.get_line());
    set_column(token.get_column());
}
