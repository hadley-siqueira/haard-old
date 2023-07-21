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
        void build_structs(Module* module);
        void build_functions(Module* module);
        void build_methods(CompoundTypeDescriptor* decl);

        void build_import(Import* import);
        void build_class(Class* decl);
        void build_struct(Struct* decl);

        void build_function(Function* function);
        void build_method(Function* method);
        void build_parameters(Function* function);
        void build_parameter(Variable* param, int idx);
        void build_function_self_type(Function* function);

    private:
        void build_template_header(TemplateHeader* templates);
        void build_self_type(CompoundTypeDescriptor* desc);

    };
}

#endif
