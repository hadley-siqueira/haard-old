#include <sstream>
#include "ast/call.h"

using namespace haard;


Call::Call(Expression* object, ExpressionList* arguments) {
    set_kind(EXPR_CALL);
    set_object(object);
    set_arguments(arguments);
}

Call::Call(Token& token, Expression* object, ExpressionList* arguments) {
    set_kind(EXPR_CALL);
    set_object(object);
    set_arguments(arguments);
    set_from_token(token);
}

Call::~Call() {
    delete object;
    delete arguments;
}

Expression* Call::get_object() const {
    return object;
}

void Call::set_object(Expression* value) {
    object = value;
}

ExpressionList* Call::get_arguments() const {
    return arguments;
}

void Call::set_arguments(ExpressionList* value) {
    arguments = value;
}

Function* Call::get_function() const {
    return function;
}

void Call::set_function(Function* value) {
    function = value;
}
