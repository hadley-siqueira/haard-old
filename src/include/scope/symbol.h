#ifndef HAARD_SYMBOL_H
#define HAARD_SYMBOL_H

#include <vector>
#include <sstream>
#include <string>
#include "ast/type.h"
#include "ast/type_list.h"
#include "ast/template_header.h"

namespace haard {
    class Scope;
    class Symbol {
        public:
            Symbol();
            Symbol(int kind, std::string name, void* descriptor);

        public:
            int get_kind();
            std::string get_name();

            void* get_descriptor();

            Type* get_type();
            int get_size_in_bytes();
            int get_alignment();

            void set_kind(int kind);
            void set_name(std::string name);

            std::string to_str();
            std::string get_qualified_name();

            Scope* get_descriptor_scope(int idx=0);

            bool has_template(TemplateHeader *types);
            void add_template(TypeList* types);

    private:
            int kind;
            std::string name;
            void* descriptor;
            std::vector<TypeList*> templates;
    };
}

#endif
