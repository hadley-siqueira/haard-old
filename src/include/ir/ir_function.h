#ifndef HAARD_IR_FUNCTION_H
#define HAARD_IR_FUNCTION_H

#include <vector>
#include <string>
#include "ir/ir_context.h"

namespace haard {
    class IRFunction : public IR {
    public:
        IRFunction();
        ~IRFunction();

    public:
        void set_name(std::string name);
        IRContext* get_context();

        std::string get_name() const;
        int parameters_count();
        IRValue* get_parameter(int idx);

        void add_parameter(IRValue* param);
        int temp_count();
        IRValue* get_temp(int idx);

    private:
        std::string name;
        std::vector<IRValue*> params;
        IRContext* ctx;
    };
}

#endif
