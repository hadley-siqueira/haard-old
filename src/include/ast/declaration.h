#ifndef HAARD_AST_DECLARATION_H
#define HAARD_AST_DECLARATION_H

#include <string>
#include <vector>
#include "ast/ast_node.h"
#include "ast/template_header.h"

namespace haard {
    class Module;

    class Declaration : public AstNode {
    public:
        Declaration();
        virtual ~Declaration();

    public:
        const std::string& get_name() const;
        void set_name(const std::string& newName);

        Module* get_module() const;
        void set_module(Module* newModule);

        TemplateHeader* get_template_header();
        void set_template_header(TemplateHeader* header, bool is_template=false);

        bool is_type_declaration();
        bool is_function();
        bool is_template();

        Declaration* get_templates(TemplateHeader* types);
        std::string get_with_templates(TypeList* types);
        std::string get_original();
        std::string get_path();
        void add_instance(Declaration* decl);

    private:
        std::string name;
        TemplateHeader* template_header;
        Module* module;
        std::vector<Declaration*> instances;
    };
}
#endif
