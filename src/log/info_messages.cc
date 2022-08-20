#include <sstream>
#include "log/info_messages.h"

using namespace haard;

std::string haard::info_message_defining_source(Source* source) {
    std::stringstream msg;

    msg << "<blue>info: </blue>defining entities for source file '" << source->get_path() << "'";
    return msg.str();
}

std::string haard::info_message_defining_class(Class* klass) {
    std::stringstream msg;

    msg << "<blue>info: </blue>defining class '" << klass->get_name() << "' on source file '" << klass->get_source()->get_path();
    return msg.str();
}

std::string haard::info_message_defining_function(Function* function) {
    std::stringstream msg;

    msg << "<blue>info: </blue>defining function '" << function->get_type_signature() << "' on source file '" << function->get_source()->get_path();
    return msg.str();

}

std::string haard::info_message_defining_method(Function* method) {
    std::stringstream msg;

    msg << "<blue>info: </blue>defining method '" << method->get_type_signature() 
        << "' on class '" << method->get_class()->get_name() << "'";
    return msg.str();

}
