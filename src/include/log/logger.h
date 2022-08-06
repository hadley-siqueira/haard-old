#ifndef HAARD_LOGGER_H
#define HAARD_LOGGER_H

#include <vector>
#include <string>
#include "log/log.h"

namespace haard {
    class Logger {
        public:
            ~Logger();

        public:
            void add_log(Log* log);
            void info(std::string path, int line, int column, std::string msg);
            void warn(std::string path, int line, int column, std::string msg);
            void error(std::string path, int line, int column, std::string msg);
            std::string read_file(const char* path, int lbegin, int count=1);
            std::string do_message(std::string buf, int line, int column, int count=1);

        private:
            std::vector<Log*> logs;
    };
}

#endif
