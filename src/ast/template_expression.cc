#include "ast/template_expression.h"

using namespace haard;

TemplateExpression::TemplateExpression() {
    kind = EXPR_TEMPLATE;
    types = nullptr;
    expression = nullptr;
}

TemplateExpression::TemplateExpression(Expression* expr, TypeList* types) {
    kind = EXPR_TEMPLATE;
    this->types = types;
    expression = expr;
}

TemplateExpression::~TemplateExpression() {
    delete expression;
}

void TemplateExpression::set_expression(Expression* expression) {
    this->expression = expression;
}

Expression* TemplateExpression::get_expression() {
    return expression;
}

void TemplateExpression::set_types(TypeList* types) {
    this->types = types;
}

TypeList* TemplateExpression::get_types() {
    return types;
}

Expression* TemplateExpression::clone() {
    TemplateExpression* other = new TemplateExpression();

    other->kind = kind;

    if (expression) {
        other->expression = expression->clone();
    }

    if (types) {
        other->types = (TypeList*) types->clone();
    }

    return other;
}

