#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <map>
#include <cstring>
#include "defs.h"
#include "log/logger.h"
#include "log/utils.h"

using namespace haard;

std::string colorify(std::string msg);

Logger::Logger() {
    error_flag = false;
    info_flag = false;
    warn_flag = false;
    internal_flag = false;
}

Logger::~Logger() {
    for (int i = 0; i < logs.size(); ++i) {
        delete logs[i];
    }
}

void Logger::print() {
    for (int i = 0; i < logs.size(); ++i) {
        std::cerr << colorify(logs[i]->to_str()) << '\n';
    }
}

void Logger::log_all() {
    error_flag = false;
    info_flag = true;
    warn_flag = true;
    internal_flag = true;
}

void Logger::info(std::string msg) {
    if (info_flag) {
        msg = "<blue>info</blue>: " + msg;
        logs.push_back(new Log(LOG_INFO, msg));
    }
}

void Logger::internal(std::string msg) {
    if (internal_flag) {
        logs.push_back(new Log(LOG_INTERNAL, msg));
    }
}

void Logger::error(std::string msg) {
    error_flag = true;
    msg = "<red>error</red>: " + msg;
    logs.push_back(new Log(LOG_ERROR, msg));
}

void Logger::error_and_exit(std::string msg) {
    error_flag = true;
    error(msg);
    print();
    exit(0);
}

bool Logger::has_error() {
    return error_flag;
}

bool Logger::logging_info() {
    return info_flag;
}
