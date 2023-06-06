#include "log/actions.h"
#include "log/logger.h"

namespace haard {
    Logger logger;

    void log_info(std::string msg) {
        logger.info(msg);
    }

    void log_error(std::string msg) {
        logger.error(msg);
    }

    void log_error_and_exit(std::string msg) {
        logger.error_and_exit(msg);
    }

    void log_print() {
        logger.print();
    }

    bool log_has_error() {
        return logger.has_error();
    }
}
