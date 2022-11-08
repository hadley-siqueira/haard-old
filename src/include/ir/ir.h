#ifndef HAARD_IR_H
#define HAARD_IR_H

#include <string>
#include "defs.h"

namespace haard {
    class IR {
        public:
            virtual ~IR();

        public:
            void set_kind(int kind);
            int get_kind();

        public:
            virtual std::string to_str();
            virtual std::string to_cpp();

        protected:
            int kind;
    };
}

#endif
