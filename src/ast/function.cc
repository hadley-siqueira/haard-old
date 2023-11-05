#include <iostream>
#include <fstream>
#include "ast/function.h"
#include "scope/scope.h"

using namespace haard;

int uid_counter = 0;

Function::Function() {
    set_kind(DECL_FUNCTION);

    return_type = nullptr;
    statements = nullptr;
    scope = new Scope();
    method_flag = false;
    virtual_flag = false;
    self_type = nullptr;
    constructor_flag = false;
    destructor_flag = false;
    uid = uid_counter++;
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
        ss << compound->get_qualified_name() << "#" << get_name();
    } else {
        ss << get_module()->get_relative_path() << "." << get_name();
    }

    if (get_template_header()) {
        ss << get_template_header()->get_qualified_name();
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

    file.seekg(get_begin());
    counter = get_begin();

    while (counter < get_end() && file.get(c)) {
        buffer += c;
        ++counter;
    }

    buffer += "\n";

    file.close();
    return buffer;
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
    set_line(token.get_line());
    set_column(token.get_column());
    set_name(token.get_lexeme());
}

void Function::set_method(bool value) {
    method_flag = value;
}

bool Function::is_template() {
    if (get_template_header()) {
        return get_template_header()->is_template();
    }

    return false;
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

std::string Function::get_type_signature() {
    int i;
    std::stringstream ss;

    ss << get_name();
    ss << " :: " << self_type->to_str();
    return ss.str();
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
    if (is_method()) {
        return compound->get_full_filepath();
    }

    return get_module()->get_path();
}

CompoundTypeDescriptor* Function::get_compound() const {
    return compound;
}

void Function::set_compound(CompoundTypeDescriptor* value) {
    compound = value;
}
