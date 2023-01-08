#ifndef HAARD_IR_CALL_H
#define HAARD_IR_CALL_H

#include <vector>
#include "ir.h"
#include "ir_value.h"

namespace haard {
    class IRCall : public IR {
        public:
            IRCall();

        public:
            void add_argument(IRValue* value);
            int arguments_count();
            IRValue* get_argument(int idx);

            std::string get_name() const;
            void set_name(const std::string& value);

            IRValue* get_dst() const;
            void set_dst(IRValue* value);

    private:
            std::string name;
            std::vector<IRValue*> args;
            IRValue* dst;
    };
}

#endif
