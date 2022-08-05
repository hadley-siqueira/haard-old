#include "utils/string_pool.h"

using namespace haard;

std::set<std::string> StringPool::pool;

const char* StringPool::get(const char* value) {
    std::string s(value);

    return pool.insert(s).first->c_str();
}

const char* StringPool::get(std::string value) {
    return pool.insert(value).first->c_str();
}
