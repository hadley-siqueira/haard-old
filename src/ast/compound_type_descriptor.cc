#include <sstream>
#include "ast/module.h"
#include "scope/scope.h"
#include "ast/compound_type_descriptor.h"

using namespace haard;

CompoundTypeDescriptor::CompoundTypeDescriptor() {
    self_type = nullptr;
    template_header = nullptr;
    template_flag = false;
    super_type = nullptr;
    destructor = nullptr;
    scope = new Scope();
}

CompoundTypeDescriptor::~CompoundTypeDescriptor() {
    for (int i = 0; i < methods.size(); ++i) {
        delete methods[i];
    }

    for (int i = 0; i < fields.size(); ++i) {
        delete fields[i];
    }

    delete scope;
    delete template_header;
}

int CompoundTypeDescriptor::get_kind() const {
    return kind;
}

int CompoundTypeDescriptor::get_line() const {
    return line;
}

int CompoundTypeDescriptor::get_column() const {
    return column;
}

std::vector<Annotation*> CompoundTypeDescriptor::get_annotations() const {
    return annotations;
}

std::string CompoundTypeDescriptor::get_name() const {
    return name;
}

std::string CompoundTypeDescriptor::get_qualified_name() {
    int i;
    std::stringstream ss;

    ss << module->get_relative_path() << "." << name;

    if (template_header) {
        ss << template_header->get_qualified_name();
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

void CompoundTypeDescriptor::set_kind(int value) {
    kind = value;
}

void CompoundTypeDescriptor::set_line(int value) {
    line = value;
}

void CompoundTypeDescriptor::set_column(int value) {
    column = value;
}

void CompoundTypeDescriptor::set_template(bool value) {
    template_flag = value;
}

void CompoundTypeDescriptor::set_annotations(const std::vector<Annotation*>& value) {
    annotations = value;
}

void CompoundTypeDescriptor::set_name(const std::string &value) {
    name = value;
}

bool CompoundTypeDescriptor::is_template() {
    return template_flag;
}

int CompoundTypeDescriptor::methods_count() {
    return methods.size();
}

int CompoundTypeDescriptor::constructors_count() {
    return constructors.size();
}

void CompoundTypeDescriptor::add_field(Field* field) {
    fields.push_back(field);
}

void CompoundTypeDescriptor::add_method(Function* method) {
    methods.push_back(method);

    if (method->get_name() == "init") {
        constructors.push_back(method);
        method->set_constructor(true);
    }

    if (method->get_name() == "destroy") {
        destructor = method;
        method->set_destructor(true);
    }

    method->set_method();
    method->set_compound(this);
    method->get_scope()->set_parent(get_scope());
}

Scope *CompoundTypeDescriptor::get_scope() const {
    return scope;
}

std::string CompoundTypeDescriptor::get_full_filepath() {
    return module->get_path();
}

std::string CompoundTypeDescriptor::get_relative_filepath() {
    return module->get_relative_path();
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

Module* CompoundTypeDescriptor::get_module() const {
    return module;
}

void CompoundTypeDescriptor::set_module(Module* value) {
    module = value;
}

TemplateHeader* CompoundTypeDescriptor::get_template_header() const {
    return template_header;
}

CompoundTypeDescriptor* CompoundTypeDescriptor::get_super_descriptor() {
    CompoundTypeDescriptor* super = nullptr;

    if (get_super_type()) {
        super = (CompoundTypeDescriptor*) ((NamedType*) super_type)->get_symbol()->get_descriptor();
    }

    return super;
}

void CompoundTypeDescriptor::set_template_header(TemplateHeader* value, bool is_template) {
    template_header = value;
    template_header->set_template_flag(is_template);
}

int CompoundTypeDescriptor::get_begin() const {
    return begin;
}

void CompoundTypeDescriptor::set_begin(int value) {
    begin = value;
}

int CompoundTypeDescriptor::get_end() const {
    return end;
}

void CompoundTypeDescriptor::set_end(int value) {
    end = value;
}

void CompoundTypeDescriptor::set_from_token(Token& token) {
    set_name(token.get_lexeme());
    set_line(token.get_line());
    set_column(token.get_column());
}
