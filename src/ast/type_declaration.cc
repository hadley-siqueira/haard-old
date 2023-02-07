#include <sstream>
#include "ast/source.h"
#include "scope/scope.h"
#include "ast/type_declaration.h"

using namespace haard;

TypeDeclaration::TypeDeclaration() {
    self_type = nullptr;
    template_header = nullptr;
    scope = new Scope();
    template_flag = false;
}

TypeDeclaration::~TypeDeclaration() {
    for (int i = 0; i < methods.size(); ++i) {
        delete methods[i];
    }

    for (int i = 0; i < fields.size(); ++i) {
        delete fields[i];
    }

    delete scope;
}

int TypeDeclaration::get_kind() const {
    return kind;
}

int TypeDeclaration::get_line() const {
    return line;
}

int TypeDeclaration::get_column() const {
    return column;
}

std::vector<Annotation*> TypeDeclaration::get_annotations() const {
    return annotations;
}

std::string TypeDeclaration::get_name() const {
    return name;
}

std::string TypeDeclaration::get_qualified_name() {
    int i;
    std::stringstream ss;

    ss << source->get_relative_path() << "." << name;

    if (template_header && template_header->types_count() > 0) {
        ss << "<";

        if (is_template()) {
            ss << template_header->types_count();
        } else {
            for (i = 0; i < template_header->types_count() - 1; ++i) {
                ss << template_header->get_type(i)->get_qualified_name() << ", ";
            }

            ss << template_header->get_type(i)->get_qualified_name();
        }

        ss << ">";
    }

    return ss.str();
}

int TypeDeclaration::get_size_in_bytes() {
    return size_in_bytes;
}

int TypeDeclaration::get_alignment() {
    return alignment;
}

void TypeDeclaration::set_kind(int value) {
    kind = value;
}

void TypeDeclaration::set_line(int value) {
    line = value;
}

void TypeDeclaration::set_column(int value) {
    column = value;
}

void TypeDeclaration::set_template(bool value) {
    template_flag = value;
}

void TypeDeclaration::set_annotations(const std::vector<Annotation*>& value) {
    annotations = value;
}

void TypeDeclaration::set_name(const std::string &value) {
    name = value;
}

bool TypeDeclaration::is_template() {
    return template_flag;
}

int TypeDeclaration::methods_count() {
    return methods.size();
}

void TypeDeclaration::add_field(Field* field) {
    fields.push_back(field);
}

void TypeDeclaration::add_method(Function* method) {
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
    //method->set_class(this);
    method->get_scope()->set_parent(get_scope());
}

Scope *TypeDeclaration::get_scope() const {
    return scope;
}

std::string TypeDeclaration::get_full_filepath() {
    return source->get_path();
}

std::string TypeDeclaration::get_relative_filepath() {
    return source->get_relative_path();
}

void TypeDeclaration::set_scope(Scope *value) {
    scope = value;
}

Type* TypeDeclaration::get_super_type() const {
    return super_type;
}

void TypeDeclaration::set_super_type(Type* value) {
    super_type = value;
}

Source* TypeDeclaration::get_source() const {
    return source;
}

void TypeDeclaration::set_source(Source* value) {
    source = value;
}

TypeList *TypeDeclaration::get_template_header() const {
    return template_header;
}

void TypeDeclaration::set_template_header(TypeList *value) {
    template_header = value;
}

int TypeDeclaration::get_begin() const {
    return begin;
}

void TypeDeclaration::set_begin(int value) {
    begin = value;
}

int TypeDeclaration::get_end() const {
    return end;
}

void TypeDeclaration::set_end(int value) {
    end = value;
}

void TypeDeclaration::set_from_token(Token& token) {
    set_name(token.get_lexeme());
    set_line(token.get_line());
    set_column(token.get_column());
}
