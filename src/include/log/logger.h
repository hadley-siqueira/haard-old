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
            void error(std::string msg);
            void error_and_exit(std::string msg);
            void print();
            void info(std::string path, int line, int column, std::string msg);
            void warn(std::string path, int line, int column, std::string msg);
            void error(Log* log);

            bool has_error();

        private:
            std::string read_line(const char* path, int line);
            std::string do_message(std::string buf, std::string color, int line, int column, int count=1);

        private:
            std::vector<Log*> logs;
            bool error_flag;
    };
}

#endif
