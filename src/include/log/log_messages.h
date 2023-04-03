#ifndef HAARD_LOG_MESSAGES_H
#define HAARD_LOG_MESSAGES_H

#include <string>

namespace haard {
    void log_error(std::string msg);
    void log_error_and_exit(std::string msg);
}

#endif
