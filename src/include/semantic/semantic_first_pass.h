#ifndef HAARD_SEMANTIC_FIRST_PASS_H
#define HAARD_SEMANTIC_FIRST_PASS_H

#include "ast/ast.h"
#include "semantic/semantic_pass.h"

namespace haard {
    class SemanticFirstPass : SemanticPass {
    public:
        void build_modules(Modules* modules);
        void build_module(Module* module);

    private:
        void set_qualified_scope(Module* module);
        void connect_sibling_scopes(Module* module);
        void define_types(Module* module);
        void define_type(CompoundTypeDescriptor* decl, int kind, std::string msg);
        void define_template_header(CompoundTypeDescriptor* decl);

    };
}

#endif
