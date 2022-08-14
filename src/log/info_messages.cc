#include "log/info_messages.h"

using namespace haard;

std::string haard::info_message_defining_source(Source* source) {
    std::stringstream msg;

    msg << "defining scope for '" << source->get_path() << "'";

    return msg.str();
}
