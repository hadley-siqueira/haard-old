#include <sstream>
#include "log/info_messages.h"

using namespace haard;

std::string haard::info_message_defining_source(Source* source) {
    std::stringstream msg;

    msg << "defining entities for source file '" << source->get_path() << "'";
    return msg.str();
}

std::string haard::info_message_defining_class(Class* klass) {
    std::stringstream msg;

    msg << " - defining class '" << klass->get_name() << "'";
    return msg.str();
}
