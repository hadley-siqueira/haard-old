#ifndef HAARD_SYMBOL_DESCRIPTOR_H
#define HAARD_SYMBOL_DESCRIPTOR_H

#include <string>

namespace haard {
    class Type;

    class SymbolDescriptor {
    public:
        SymbolDescriptor();
        SymbolDescriptor(int kind, void* descriptor);

    public:
        int get_kind() const;
        void set_kind(int value);

        void* get_descriptor() const;
        void set_descriptor(void* value);

        Type* get_type();
        std::string to_str();
        std::string get_qualified_name();

        int get_size_in_bytes();
        int get_alignment();

        Scope* get_descriptor_scope();

    private:
        int kind;
        void* descriptor;
    };
}

#endif
