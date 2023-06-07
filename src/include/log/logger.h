#ifndef HAARD_LOGGER_H
#define HAARD_LOGGER_H

#include <vector>
#include <string>
#include "log/log.h"
#include "token/token.h"

namespace haard {
    class Logger {
    public:
        Logger();
        ~Logger();

    public:
        void info(std::string msg);
        void internal(std::string msg);
        void error(std::string msg);
        void error_and_exit(std::string msg);
        void print();
        bool has_error();

    private:
        std::vector<Log*> logs;
        bool error_flag;
        bool info_flag;
        bool warn_flag;
        bool internal_flag;
    };
}

#endif
