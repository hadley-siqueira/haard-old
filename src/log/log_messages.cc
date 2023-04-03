#include "log/log_messages.h"
#include "log/logger.h"

using namespace haard;

Logger logger;

void haard::log_error(std::string msg) {
    logger.error(msg);
}

void haard::log_error_and_exit(std::string msg) {
    logger.error_and_exit(msg);
}
