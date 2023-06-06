#ifndef HAARD_AST_STATEMENT_H
#define HAARD_AST_STATEMENT_H

#include "ast_node.h"

namespace haard {
    class Scope;

    class Statement : public AstNode {
    public:
        Statement();
        virtual ~Statement();

        Scope* get_scope() const;
        void set_scope(Scope* value);

    private:
        Scope* scope;
    };
}

#endif
