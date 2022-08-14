#include <sstream>
#include "log/log.h"
#include "defs.h"

using namespace haard;

Log::Log(int kind, std::string message) {
    this->kind = kind;
    this->message = message;
}

Log::Log(int kind, int line, int column, std::string path, std::string message) {
    this->kind = kind;
    this->line = line;
    this->column = column;
    this->path = path;
    this->message = message;
}

std::string Log::to_str() {
    std::stringstream ss;
/*
    ss << path << ':' << line << ':' << column << ": ";

    switch (kind) {
    case LOG_WARNING:
        ss << MAGENTA << "warning: " << NORMAL;
        break;

    case LOG_INFO:
        ss << BLUE << "info: " << NORMAL;
        break;

    case LOG_ERROR:
        ss << RED << "error: " << NORMAL;
        break;
    }*/

    ss << message;

    return ss.str();
}
