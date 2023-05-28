#include "ast/call.h"

using namespace haard;


int Call::get_line() const
{
    return line;
}

void Call::set_line(int value)
{
    line = value;
}

int Call::get_column() const
{
    return column;
}

void Call::set_column(int value)
{
    column = value;
}

Expression *Call::get_object() const
{
    return object;
}

void Call::set_object(Expression *value)
{
    object = value;
}

ExpressionList *Call::get_arguments() const
{
    return arguments;
}

void Call::set_arguments(ExpressionList *value)
{
    arguments = value;
}
