#ifndef HAARD_LOG_H
#define HAARD_LOG_H

#include <string>

namespace haard {
    class Log {
        public:
            Log(int kind, std::string message);
            Log(int kind, int line, int column, std::string path, std::string message);
            std::string to_str();

        private:
            int kind;
            int line;
            int column;
            std::string path;
            std::string message;
    };
}

#endif
