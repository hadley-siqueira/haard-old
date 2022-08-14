#include <sstream>
#include <cstring>
#include "log/messages.h"
#include "log/error_messages.h"

using namespace haard;

/*
file.hd:10:20: error: can't define class <white>name</white>
    10 | class Window:
               ^^^^^^

file.hd:5:20: note: already defined as function <white>name</white>
    5 | def Window
 */

std::string haard::error_message_cant_define_class(Class* klass, Symbol* sym) {
    std::stringstream msg;
    std::string path = klass->get_source()->get_path();
    int line = klass->get_line();
    int column = klass->get_column();
    int count = strlen(klass->get_name());

    msg << error_header(path, line, column);
    msg << "can't define class <white>\"" << klass->get_name() << "\"</white>\n";
    msg << get_line_for_message(path, line, column, count);
    msg << "first occurence on \n";
    return msg.str();
}
