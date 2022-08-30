#ifndef HAARD_IR_VALUE_H
#define HAARD_IR_VALUE_H

#include <string>

namespace haard {
    class IRValue {
        public:
            IRValue();
            IRValue(int kind, const char* value);
            IRValue(int kind, int tmp);

        public:
            std::string to_str();

        private:
            int kind;
            const char* value;
    };
}

#endif
