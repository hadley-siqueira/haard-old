#ifndef HAARD_SYMBOL_H
#define HAARD_SYMBOL_H

#include <vector>
#include <sstream>
#include <string>
#include "ast/type.h"
#include "ast/type_list.h"
#include "symbol_descriptor.h"
#include "ast/template_header.h"

namespace haard {
    class Scope;
    class Symbol {
    public:
        Symbol();
        Symbol(std::string name);
        Symbol(int kind, std::string name, SymbolDescriptor* descriptor);
        ~Symbol();

    public:
        int get_kind();
        std::string get_name();
        std::string to_str(int idx=0);

        Type* get_type(int idx=0);
        int get_size_in_bytes(int idx=0);
        int get_alignment(int idx=0);

        void set_kind(int kind);
        void set_name(std::string name);

        std::string get_qualified_name(int idx=0);

        Scope* get_descriptor_scope(int index=0);

        SymbolDescriptor* get_descriptor(int idx=0);
        void add_descriptor(SymbolDescriptor* descriptor);
        int descriptors_count();

    private:
        int kind;
        std::string name;
        std::vector<SymbolDescriptor*> descriptors;
    };
}

#endif
