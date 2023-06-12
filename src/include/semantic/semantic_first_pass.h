#ifndef HAARD_SEMANTIC_FIRST_PASS_H
#define HAARD_SEMANTIC_FIRST_PASS_H

#include "ast/ast.h"
#include "semantic/semantic_pass.h"

namespace haard {
    class SemanticFirstPass : SemanticPass {
    public:
        void build_modules(Modules* modules);
        void build_module(Module* module);

        void build_imports(Module* module);
        void build_classes(Module* module);

        void build_import(Import* import);
        void build_class(Class* decl);

    private:
        void define_template_header(CompoundTypeDescriptor* decl);

    };
}

#endif
