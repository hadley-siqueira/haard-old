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

            int get_kind() const;
            void set_kind(int value);

            const char* get_value() const;
            void set_value(const char* value);
            uint64_t to_u64();
;
    private:
            int kind;
            const char* value;
    };
}

#endif
