#ifndef HAARD_LOG_UTILS_H
#define HAARD_LOG_UTILS_H

#include <string>
#include "defs.h"

namespace haard {
    std::string read_line(std::string path, int line);
    std::string colorify(std::string msg);
    std::string create_trailing(int line, int column, int count);
    std::string extract_line(std::string path, int line, int column, int count);
}

#endif
