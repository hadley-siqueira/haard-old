#ifndef HAARD_SYMBOL_H
#define HAARD_SYMBOL_H

#include <vector>
#include <sstream>
#include <string>
#include "ast/type.h"
#include "ast/type_list.h"

namespace haard {
    class Symbol {
        public:
            Symbol();
            Symbol(int kind, const char* name, void* descriptor);

        public:
            int get_kind();
            const char* get_name();
            void* get_descriptor(int idx=0);
            Type* get_type(int idx=0);

            void set_kind(int kind);
            void set_name(const char* name);
            void add_descriptor(void* descriptor);

            std::string to_str(int idx=0);
            std::string to_cpp(int idx=0);

            int overloaded_count();
            void* get_overloaded(TypeList* types);

        private:
            int kind;
            const char* name;
            std::vector<void*> descriptors;
    };
}

#endif
