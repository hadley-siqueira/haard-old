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
    std::string path = klass->get_module()->get_path();
    int line = klass->get_line();
    int column = klass->get_column();
    int count = klass->get_name().size();

    msg << error_header(path, line, column);
    msg << "can't define class <white>\"" << klass->get_name() << "\"</white>\n";
    msg << get_line_for_message(path, line, column, count);
    msg << "first occurence on \n";
    return msg.str();
}

std::string haard::error_message_id_not_in_scope(Module* module, Identifier* id) {
    std::stringstream msg;
    std::string path = module->get_path();
    int line = id->get_line();
    int column = id->get_column();
    int count = id->get_name().size();

    msg << error_header(path, line, column);
    msg << "<white>'" << id->get_name() << "'</white> not in scope\n";
    msg << get_line_for_message(path, line, column, count);

    return msg.str();
}
