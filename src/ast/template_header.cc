#include <sstream>
#include "ast/template_header.h"

using namespace haard;

void TemplateHeader::add_type(Type* type) {
    types.push_back(type);
}

int TemplateHeader::types_count() {
    return types.size();
}

Type* TemplateHeader::get_type(int idx) {
    if (idx < types.size()) {
        return types[idx];
    }

    return nullptr;
}

TemplateHeader* TemplateHeader::clone() {
    TemplateHeader* other = new TemplateHeader();

    for (int i = 0; i < types_count(); ++i) {
        other->add_type(get_type(i)->clone());
    }

    return other;
}

std::string TemplateHeader::to_str() {
    int i;
    std::stringstream ss;

    ss << '<';

    for (i = 0; i < types.size() - 1; ++i) {
        ss << types[i]->to_str() << ", ";
    }

    ss << types[i]->to_str();
    ss << '>';
    return ss.str();
}

