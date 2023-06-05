#ifndef HAARD_AST_EXPRESSION_H
#define HAARD_AST_EXPRESSION_H

#include "defs.h"
#include "ast/type.h"
#include "ast/ast_node.h"

namespace haard {
    class Expression : public AstNode {
    public:
        Expression();
        virtual ~Expression();

    public:
        Type* get_type();
        void set_type(Type* type);

    protected:
        Type* type;
    };
}

#endif
