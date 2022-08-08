#ifndef HAARD_LOGGER_H
#define HAARD_LOGGER_H

#include <vector>
#include <string>
#include "log/log.h"
#include "token/token.h"

namespace haard {
    class Logger {
        public:
            ~Logger();

        public:
            void print();
            void info(std::string path, int line, int column, std::string msg);
            void warn(std::string path, int line, int column, std::string msg);
            void error(Log* log);

        private:
            std::string read_line(const char* path, int line);
            std::string do_message(std::string buf, std::string color, int line, int column, int count=1);

        private:
            std::vector<Log*> logs;
    };
}

#endif
