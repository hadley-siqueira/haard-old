#ifndef HDC_UTILS_STRING_POOL_H
#define HDC_UTILS_STRING_POOL_H

#include <set>
#include <string>

namespace haard {
    class StringPool {
        public:
            static const char* get(const char* value);
            static const char* get(std::string value);
            static void debug();

        public:
            static std::set<std::string> pool;
    };
}

#endif

