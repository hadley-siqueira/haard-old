#include <iostream>
#include "ast/function.h"
#include "scope/scope.h"

using namespace haard;

Function::Function() {
    name = nullptr;
    return_type = nullptr;
    statements = nullptr;
    scope = new Scope();
    method_flag = false;
    templates = nullptr;
    self_type = nullptr;
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

const char* Function::get_name() {
    return name;
}

std::string Function::get_cpp_name() {
    std::stringstream ss;

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

    ss << return_type->to_cpp();
    ss << ' ';
    ss << get_cpp_name() << '(';

    if (parameters_count() > 0) {
        int i;

        for (i = 0; i < parameters_count() - 1; ++i) {
            ss << get_parameter(i)->get_type()->to_cpp();
            ss << ' ' << get_parameter(i)->get_cpp_name() << ", ";
        }

        ss << get_parameter(i)->get_type()->to_cpp();
        ss << ' ' << get_parameter(i)->get_cpp_name();
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

void Function::set_line(int line) {
    this->line = line;
}

void Function::set_column(int column) {
    this->column = column;
}

void Function::set_name(const char* name) {
    this->name = name;
}

void Function::set_return_type(Type* type) {
    return_type = type;
}

void Function::set_statements(CompoundStatement* statements) {
    this->statements = statements;
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

void Function::set_self_type(Type* type) {
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

void Function::set_template_list(TypeList* types) {
    this->templates = types;
}

TypeList* Function::get_template_list() {
    return templates;
}

bool Function::is_template() {
    if (templates == nullptr) {
        return false;
    }

    for (int i = 0; i < templates->types_count(); ++i) {
        TemplateType* type = (TemplateType*) templates->get_type(i);

        if (!type->is_binded()) {
            return true;
        }
    }

    return false;
}

Function* Function::clone() {
    Function* nfunc = new Function();

    nfunc->uid = uid;
    nfunc->line = line;
    nfunc->column = column;
    nfunc->method_flag = method_flag;
    nfunc->name = name;
    nfunc->scope->set_parent(scope->get_parent());
    
    for (int i = 0; i < parameters.size(); ++i) {
        nfunc->add_parameter(parameters[i]->clone());
    }

    for (int i = 0; i < variables.size(); ++i) {
        nfunc->add_variable(variables[i]->clone());
    }

    if (templates) {
        nfunc->templates = (TypeList*) templates->clone();
    }

    if (return_type) {
        nfunc->return_type = return_type->clone();
    }

    if (self_type) {
        nfunc->self_type = self_type->clone();
    }

    if (statements) {
        nfunc->statements = (CompoundStatement*) statements->clone();
    }

    return nfunc;
}

bool Function::is_binded_with_types(TypeList* types) {
    if (templates == nullptr) {
        return false;
    }

    if (templates->types_count() != types->types_count()) {
        return false;
    }

    for (int i = 0; i < templates->types_count(); ++i) {
        TemplateType* t = (TemplateType*) templates->get_type(i);

        if (t->is_binded()) {
            if (!t->get_bind_type()->equal(types->get_type(i))) {
                return false;
            }
        }
    }

    return true;
}

void Function::bind_with(TypeList* types) {
    for (int i = 0; i < templates->types_count(); ++i) {
        TemplateType* t = (TemplateType*) templates->get_type(i);
        t->set_bind_type(types->get_type(i));
    }
}

Function* Function::get_with_template_binding(TypeList* bindings) {
    for (int i = 0; i < tfunctions.size(); ++i) {
        if (tfunctions[i]->is_binded_with_types(bindings)) {
            return tfunctions[i];
        }
    }

    Function* new_func = clone();

    new_func->bind_with(bindings);
    this->tfunctions.push_back(new_func);

    return tfunctions[tfunctions.size() - 1];
}

std::string Function::get_type_signature() {
    std::stringstream ss;

    ss << name << self_type->to_str();
    return ss.str();
}
