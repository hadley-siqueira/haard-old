#include <cstring>
#include <sstream>
#include "ast/class.h"
#include "scope/scope.h"

using namespace haard;

Class::Class() {
    super_class = nullptr;
    self_type = nullptr;
    template_header = nullptr;
    destructor = nullptr;
    scope = new Scope();
    name = nullptr;
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

const char* Class::get_name() {
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

void Class::set_name(const char* name) {
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

    if (strcmp(method->get_name(), "init") == 0) {
        constructors.push_back(method);
    }
    
    if (strcmp(method->get_name(), "destroy") == 0) {
        destructor = method;
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
    Variable* var;

    for (int i = 0; i < variables_count(); ++i) {
        var = get_variable(i);
        size = var->get_type()->get_size_in_bytes();
        align = var->get_type()->get_alignment();

        while (offset % align != 0) {
            ++offset;
        }

        var->set_offset(offset);
        offset += size;
    }
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

void Class::set_template_header(TemplateHeader* header) {
    template_header = header;
}

TemplateHeader* Class::get_template_header() {
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

int Class::get_size_in_bytes() {
    int sz = 0;

    for (int i = 0; i < variables.size(); ++i) {
        sz += variables[i]->get_size_in_bytes();
    }

    return sz;
}
