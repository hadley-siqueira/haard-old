#ifndef HAARD_AST_IDENTIFIER_H
#define HAARD_AST_IDENTIFIER_H

#include <string>
#include "ast/expression.h"
#include "token/token.h"
#include "scope/symbol.h"
#include "ast/template_header.h"

namespace haard {
    class Identifier : public Expression {
    public:
        Identifier();
        Identifier(Token& token);
        Identifier(Token& alias, Token& name);
        Identifier(std::string name);
        ~Identifier();

    public:
        std::string& get_name();
        SymbolDescriptor* get_symbol_descriptor();
        void set_name(std::string name);
        void set_symbol_descriptor(SymbolDescriptor* value);

        bool is_local_variable();
        bool is_class_variable();

        bool has_scope();
        bool has_global_scope();
        std::string get_alias() const;
        void set_alias(const std::string& value);

        TemplateHeader* get_template_header() const;
        void set_template_header(TemplateHeader* value);

        bool has_template();

        std::string to_str();

        int get_symbol_index() const;
        void set_symbol_index(int value);

    private:
        std::string name;
        std::string alias;
        SymbolDescriptor* symbol_descriptor;
        int symbol_index;
        TemplateHeader* template_header;
            
    };
}

#endif
