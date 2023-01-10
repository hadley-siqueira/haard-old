#ifndef HAARD_IR_VALUE_H
#define HAARD_IR_VALUE_H

#include <string>

namespace haard {
    class IRValue {
        public:
            IRValue();
            IRValue(int kind, std::string& value);
            IRValue(int kind, int tmp);

        public:
            std::string to_str();

            int get_kind() const;
            void set_kind(int value);

            std::string get_value() const;
            void set_value(const char* value);
            uint64_t to_u64();
;
    private:
            int kind;
            std::string value;
    };
}

#endif
