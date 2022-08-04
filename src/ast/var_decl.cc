#include "ast/var_decl.h"

using namespace haard;

VarDeclaration::VarDeclaration() {
    kind = STMT_VAR_DECL;
    expr = nullptr;
    var = nullptr;
}

VarDeclaration::~VarDeclaration() {
    delete expr;
    delete var;
}

Variable* VarDeclaration::get_variable() {
    return var;
}

Expression* VarDeclaration::get_expression() {
    return expr;
}

void VarDeclaration::set_variable(Variable* var) {
    this->var = var;
}

void VarDeclaration::set_expression(Expression* expr) {
    this->expr = expr;
}
