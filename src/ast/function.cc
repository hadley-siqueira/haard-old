#include <iostream>
#include <fstream>
#include "ast/function.h"
#include "scope/scope.h"

using namespace haard;

Function::Function() {
    return_type = nullptr;
    statements = nullptr;
    scope = new Scope();
    method_flag = false;
    virtual_flag = false;
    template_header = nullptr;
    self_type = nullptr;
    constructor_flag = false;
    destructor_flag = false;
    template_flag = false;
}

Function::~Function() {
    delete statements;
    delete scope;
    
    for (int i = 0; i < parameters.size(); ++i) {
        delete parameters[i];
    }

    for (int i = 0; i < variables.size(); ++i) {
        delete variables[i];
    }
}

int Function::get_line() {
    return line;
}

int Function::get_column() {
    return column;
}

std::string Function::get_name() {
    return name;
}

std::string Function::get_cpp_name() {
    std::stringstream ss;

    if (is_virtual()) {
        if (get_parent_method() != nullptr) {
            return get_parent_method()->get_cpp_name();
        }
    }

    if (method_flag) {
        ss << 'm';
    } else {
        ss << 'f';
    }

    ss << uid << '_' << name;
    return ss.str();
}

std::string Function::get_cpp_signature() {
    std::stringstream ss;

    if (is_virtual()) {
        if (get_parent_method() != nullptr) {
            return get_parent_method()->get_cpp_signature();
        }
    }

    if (annotations.size() > 0) {
        ss << "virtual ";
    }

    ss << return_type->to_cpp();
    ss << ' ';
    ss << get_cpp_name() << '(';

    if (parameters_count() > 0) {
        int i;

        for (i = 0; i < parameters_count() - 1; ++i) {
            ss << get_parameter(i)->get_type()->to_cpp();
            ss << ' ' << get_parameter(i)->get_unique_name() << ", ";
        }

        ss << get_parameter(i)->get_type()->to_cpp();
        ss << ' ' << get_parameter(i)->get_unique_name();
    }

    ss << ')';

    return ss.str();
}

Variable* Function::get_parameter(int idx) {
    if (idx < parameters.size()) {
        return parameters[idx];
    }

    return nullptr;
}

Variable* Function::get_variable(int idx) {
    if (idx < variables.size()) {
        return variables[idx];
    }

    return nullptr;
}

Type* Function::get_return_type() {
    return return_type;
}

CompoundStatement* Function::get_statements() {
    return statements;
}

Scope* Function::get_scope() {
    return scope;
}

std::string Function::get_qualified_name() {
    int i;
    std::stringstream ss;

    if (is_method()) {
        ss << klass->get_qualified_name() << "#" << name;
    } else {
        ss << source->get_relative_path() << "." << name;
    }

    if (template_header && template_header->types_count() > 0) {
        ss << "<";

        for (i = 0; i < template_header->types_count() - 1; ++i) {
            ss << template_header->get_type(i)->get_qualified_name() << ", ";
        }

        ss << template_header->get_type(i)->get_qualified_name();
        ss << ">";
    }

    ss << "(";

    if (parameters_count() > 0) {
        for (i = 0; i < parameters_count() - 1; ++i) {
            ss << get_parameter(i)->get_type()->get_qualified_name();
            ss << ", ";
        }

        ss << get_parameter(i)->get_type()->get_qualified_name();
    } else {
        ss << "void";
    }

    ss << ")";
    return ss.str();
}

std::string Function::get_original() {
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

void Function::set_line(int line) {
    this->line = line;
}

void Function::set_column(int column) {
    this->column = column;
}

void Function::set_name(std::string name) {
    this->name = name;
}

void Function::set_return_type(Type* type) {
    return_type = type;
}

void Function::set_statements(CompoundStatement* statements) {
    this->statements = statements;
}

void Function::set_virtual(bool v) {
    virtual_flag = v;
}

void Function::set_template(bool v) {
    template_flag = v;
}

int Function::get_uid() {
    return uid;
}

void Function::set_uid(int uid) {
    this->uid = uid;
}

Type* Function::get_self_type() {
    return self_type;
}

void Function::set_self_type(FunctionType* type) {
    self_type = type;
}

int Function::parameters_count() {
    return parameters.size();
}

void Function::add_parameter(Variable* param) {
    parameters.push_back(param);
    param->set_kind(VAR_PARAM);
}

void Function::add_variable(Variable* var) {
    variables.push_back(var);
}

void Function::set_from_token(Token& token) {
    line = token.get_line();
    column = token.get_column();
    name = token.get_lexeme();
}

void Function::set_method(bool value) {
    method_flag = value;
}

bool Function::is_template() {
    return template_flag;
}

bool Function::is_method() {
    return method_flag;
}

bool Function::is_constructor() {
    return constructor_flag;
}

bool Function::is_destructor() {
    return destructor_flag;
}

bool Function::is_virtual() {
    return virtual_flag;
}

Function* Function::clone() {
    Function* nfunc = new Function();

    nfunc->uid = uid;
    nfunc->line = line;
    nfunc->column = column;
    nfunc->method_flag = method_flag;
    nfunc->name = name;
    nfunc->scope->set_parent(scope->get_parent());
    nfunc->source = source;
    
    for (int i = 0; i < parameters.size(); ++i) {
        nfunc->add_parameter(parameters[i]->clone());
    }

    for (int i = 0; i < variables.size(); ++i) {
        nfunc->add_variable(variables[i]->clone());
    }

    if (template_header) {
        nfunc->template_header = (TypeList*) template_header->clone();
    }

    if (return_type) {
        nfunc->return_type = return_type->clone();
    }

    if (self_type) {
        nfunc->self_type = (FunctionType*) self_type->clone();
    }

    if (statements) {
        nfunc->statements = (CompoundStatement*) statements->clone();
    }

    return nfunc;
}

std::string Function::get_type_signature() {
    int i;
    std::stringstream ss;

    ss << name;
    ss << " :: " << self_type->to_str();
    return ss.str();
}

void Function::set_source(Source* source) {
    this->source = source;
}

Source* Function::get_source() {
    return source;
}

bool Function::same_signature(Function* other) {
    if (template_header && other->template_header) {
        if (template_header->types_count() != other->template_header->types_count()) {
            return false;
        }
    }

    if (parameters_count() != other->parameters_count()) {
        return false;
    } else {
        for (int i = 0; i < parameters_count(); ++i) {
            if (!parameters[i]->get_type()->equal(other->parameters[i]->get_type())) {
                return false;
            }
        }
    }

    return true;
}

void Function::set_overloaded_index(int idx) {
    overloaded_index = idx;
}

int Function::get_overloaded_index() {
    return overloaded_index;
}

void Function::set_template_header(TypeList* header) {
    template_header = header;
}

TypeList* Function::get_template_header() {
    return template_header;
}


void Function::set_class(Class* klass) {
    this->klass = klass;
}

Class* Function::get_class() {
    return klass;
}

std::vector<Annotation*> Function::get_annotations() const {
    return annotations;
}

void Function::set_annotations(const std::vector<Annotation*>& value) {
    annotations = value;

    for (int i = 0; i < annotations.size(); ++i) {
        if (annotations[i]->get_value().compare("virtual") == 0) {
            virtual_flag = true;
        }
    }
}

Function* Function::get_parent_method() const {
    return parent_method;
}

void Function::set_parent_method(Function* value) {
    parent_method = value;
}

void Function::set_constructor(bool value) {
    constructor_flag = value;
}

void Function::set_destructor(bool value) {
    destructor_flag = value;
}

std::string Function::get_path() {
    return source->get_path();
}

int Function::get_begin() const {
    return begin;
}

void Function::set_begin(int value) {
    begin = value;
}

int Function::get_end() const {
    return end;
}

void Function::set_end(int value) {
    end = value;
}
