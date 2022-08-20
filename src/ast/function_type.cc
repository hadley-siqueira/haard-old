#include <sstream>
#include "ast/function_type.h"

using namespace haard;

FunctionType::FunctionType() {
    kind = TYPE_FUNCTION;
    template_header = nullptr;
    rtype = nullptr;
}

FunctionType::~FunctionType() {
    delete template_header;
}

void FunctionType::add_param_type(Type* type) {
    ptypes.push_back(type);
}

void FunctionType::set_return_type(Type* type) {
    rtype = type;
}

int FunctionType::params_count() {
    return ptypes.size();
}

void FunctionType::add_template(Type* type) {
    if (template_header == nullptr) {
        template_header = new TemplateHeader();
    }

    template_header->add_type(type);
}

Type* FunctionType::get_param_type(int idx) {
    if (idx < params_count()) {
        return ptypes[idx];
    }

    return nullptr;
}

Type* FunctionType::get_return_type() {
    return rtype;
}


// override
bool FunctionType::equal(Type* type) {
    FunctionType* other = (FunctionType*) type;

    if (type == nullptr || type->get_kind() != kind) {
        return false;
    }

    if (params_count() != other->params_count()) {
        return false;
    }

    for (int i = 0; i < params_count(); ++i) {
        Type* t1 = get_param_type(i);
        Type* t2 = other->get_param_type(i);

        if (!t1->equal(t2)) {
            return false;
        }
    }

    if (!rtype->equal(other->rtype)) {
        return false;
    }

    return true;
}

Type* FunctionType::clone() {
    FunctionType* other = new FunctionType();

    for (int i = 0; i < params_count(); ++i) {
        other->add_param_type(get_param_type(i)->clone());
    }

    other->set_return_type(rtype->clone());
    other->template_header = template_header->clone();

    return other;
}

std::string FunctionType::to_str() {
    std::stringstream ss;

    if (template_header) {
        ss << template_header->to_str() << ' ';
    }

    for (int i = 0; i < params_count(); ++i) {
        ss << get_param_type(i)->to_str() << " -> ";
    }

    ss << rtype->to_str();
    return ss.str();
}

