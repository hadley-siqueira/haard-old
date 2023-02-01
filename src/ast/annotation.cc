#include "ast/annotation.h"

using namespace haard;

std::string Annotation::get_value() const {
    return value;
}

void Annotation::set_value(const std::string &value) {
    this->value = value;
}
