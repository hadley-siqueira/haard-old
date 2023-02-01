#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include "ast/class.h"
#include "scope/scope.h"

using namespace haard;

Class::Class() {
    super_class = nullptr;
    self_type = nullptr;
    template_header = nullptr;
    destructor = nullptr;
    scope = new Scope();
    is_virtual_flag = false;
    template_flag = false;
}

Class::~Class() {
    for (int i = 0; i < methods.size(); ++i) {
        delete methods[i];
    }

    for (int i = 0; i < variables.size(); ++i) {
        delete variables[i];
    }

    delete scope;
}

std::string Class::get_name() {
    return name;
}

std::string Class::get_cpp_name() {
    std::stringstream ss;

    ss << "c" << uid << "_" << name;
    return ss.str();
}

int Class::get_line() {
    return line;
}

int Class::get_column() {
    return column;
}

Function* Class::get_method(int idx) {
    if (idx < methods_count()) {
        return methods[idx];
    }

    return nullptr;
}

Variable* Class::get_variable(int idx) {
    if (idx < variables_count()) {
        return variables[idx];
    }

    return nullptr;
}

Type* Class::get_super_class() {
    return super_class;
}

Scope* Class::get_scope() {
    return scope;
}

NamedType* Class::get_self_type() {
    return self_type;
}

void Class::set_from_token(Token& token) {
    set_name(token.get_lexeme());
    set_line(token.get_line());
    set_column(token.get_column());
}

void Class::set_name(std::string name) {
    this->name = name;
}

void Class::set_line(int line) {
    this->line = line;
}

void Class::set_column(int column) {
    this->column = column;
}

void Class::set_super_class(Type* type) {
    super_class = type;
}

bool Class::has_super_class() {
    return super_class != nullptr;
}

Class* Class::get_super_class_descriptor() {
    Class* super = nullptr;

    if (has_super_class()) {
        super = (Class*) ((NamedType*) super_class)->get_symbol()->get_descriptor();
    }

    return super;
}

int Class::get_uid() {
    return uid;
}

void Class::set_uid(int uid) {
    this->uid = uid;
}

void Class::set_self_type(NamedType* type) {
    self_type = type;
}

void Class::add_method(Function* method) {
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
    method->set_class(this);
}

void Class::add_variable(Variable* var) {
    variables.push_back(var);
    var->set_kind(VAR_CLASS);
}

void Class::calculate_variables_offset() {
    int size = 0;
    int offset = 0;
    int align = 0;
    int max_align = 0;
    Variable* var;

    // if has a super class, the super class already calculated vtable pointer etc
    // if doesn't have super class, need to check if need vtable ptr
    if (has_super_class()) {
        Class* parent = get_super_class_descriptor();
        offset = super_class->get_size_in_bytes() - parent->get_remaining_pad();
        max_align = super_class->get_alignment();
    } else {
        if (is_virtual()) {
            offset = ARCH_WORD_SIZE;
            max_align = ARCH_WORD_SIZE;
        }
    }

    for (int i = 0; i < variables_count(); ++i) {
        var = get_variable(i);
        size = var->get_type()->get_size_in_bytes();
        align = var->get_type()->get_alignment();

        if (align > max_align) {
            max_align = align;
        }

        while (offset % align != 0) {
            ++offset;
        }

        var->set_offset(offset);
        offset += size;
    }

    // the remaining pad is used to calculate children class offsets
    // Maybe a child class can use the space left by the parent to
    // store one of its variables
    remaining_pad = 0;

    while (max_align != 0 && offset % max_align != 0) {
        ++offset;
        ++remaining_pad;
    }

    size_in_bytes = offset;
    alignment = max_align;
}

int Class::methods_count() {
    return methods.size();
}

int Class::variables_count() {
    return variables.size();
}

int Class::constructors_count() {
    return constructors.size();
}

Function* Class::get_constructor(int idx) {
    if (idx < constructors_count()) {
        return constructors[idx];
    }

    return nullptr;
}

Source* Class::get_source() {
    return source;
}

void Class::set_source(Source* source) {
    this->source = source;
}

void Class::set_template_header(TypeList* header) {
    template_header = header;
}

TypeList* Class::get_template_header() {
    return template_header;
}

Function* Class::get_destructor() const {
    return destructor;
}

void Class::set_destructor(Function* value) {
    destructor = value;
}

std::vector<std::string> Class::get_annotations() const {
    return annotations;
}

void Class::set_annotations(const std::vector<std::string>& value) {
    annotations = value;
}

void Class::set_alignment(int value) {
    alignment = value;
}

void Class::set_virtual(bool flag) {
    is_virtual_flag = flag;
}

bool Class::is_virtual() {
    return is_virtual_flag;
}

int Class::get_remaining_pad() const {
    return remaining_pad;
}

void Class::set_remaining_pad(int value) {
    remaining_pad = value;
}

bool Class::is_template() {
    return template_flag;
}

void Class::set_template(bool value) {
    template_flag = value;
}

std::string Class::get_path() {
    return source->get_path();
}

int Class::get_begin() const {
    return begin;
}

void Class::set_begin(int value) {
    begin = value;
}

int Class::get_end() const {
    return end;
}

void Class::set_end(int value) {
    end = value;
}

int Class::get_alignment() const {
    return alignment;
}

int Class::get_size_in_bytes() {
    return size_in_bytes;
}

std::string Class::get_qualified_name() {
    int i;
    std::stringstream ss;

    ss << source->get_relative_path() << "." << name;

    if (template_header && template_header->types_count() > 0) {
        ss << "<";

        for (i = 0; i < template_header->types_count() - 1; ++i) {
            ss << template_header->get_type(i)->get_qualified_name() << ", ";
        }

        ss << template_header->get_type(i)->get_qualified_name();
        ss << ">";
    }

    return ss.str();
}

std::string Class::get_original() {
    char c;
    std::ifstream file;
    std::string buffer;
    int counter;

    file.open(get_path());

    file.seekg(begin);
    counter = begin;

    while (counter < end && file.get(c)) {
        buffer += c;
        ++counter;
    }

    buffer += "\n";

    file.close();
    return buffer;
}
