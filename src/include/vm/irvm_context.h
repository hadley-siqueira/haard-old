#ifndef HAARD_IRVM_CONTEXT_H
#define HAARD_IRVM_CONTEXT_H

#include <map>

namespace haard {
    class IrVMContext {
        public:
            IrVMContext(uint64_t ip=0, uint64_t sp=0, uint64_t fp=0);

            uint64_t get_ip() const;
            void set_ip(const uint64_t& value);

            uint64_t get_sp() const;
            void set_sp(const uint64_t& value);

            uint64_t get_fp() const;
            void set_fp(const uint64_t& value);

            std::map<std::string, uint64_t> get_values() const;
            void set_values(const std::map<std::string, uint64_t>& value);

        private:
            uint64_t ip;
            uint64_t sp;
            uint64_t fp;
            std::map<std::string, uint64_t> values;
    };
}

#endif
