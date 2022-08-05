#ifndef HAARD_SYMBOL_H
#define HAARD_SYMBOL_H

#include <vector>

namespace haard {
    class Symbol {
        public:
            Symbol();
            Symbol(int kind, const char* name, void* descriptor);

        public:
            int get_kind();
            const char* get_name();
            void* get_descriptor(int idx=0);

            void set_kind(int kind);
            void set_name(const char* name);
            void add_descriptor(void* descriptor);

        private:
            int kind;
            const char* name;
            std::vector<void*> descriptors;
    };
}

#endif
