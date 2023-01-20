#ifndef HAARD_SYMBOL_H
#define HAARD_SYMBOL_H

#include <vector>
#include <sstream>
#include <string>
#include "ast/type.h"
#include "ast/type_list.h"

namespace haard {
    class Scope;
    class Symbol {
        public:
            Symbol();
            Symbol(int kind, std::string name, void* descriptor);

        public:
            int get_kind();
            std::string get_name();
            void* get_descriptor(int idx=0);
            Type* get_type(int idx=0);
            int get_size_in_bytes(int idx=0);
            int get_alignment(int idx=0);

            void set_kind(int kind);
            void set_name(std::string name);
            void add_descriptor(void* descriptor);

            std::string to_str(int idx=0);
            std::string to_cpp(int idx=0);

            int overloaded_count();
            int get_overloaded(TypeList* types);

            Scope* get_descriptor_scope(int idx=0);

    private:
            int kind;
            std::string name;
            std::vector<void*> descriptors;
    };
}

#endif
