#include <sstream>
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

std::string haard::error_message_class_aready_defined(Class* klass, Symbol* sym) {
    std::stringstream msg;
    std::string path = "file.hd";
    int line = 1;
    int column = 1;

    msg << error_header(path, line, column);
    msg << "can't define class <red>" << klass->get_name() << "</red>\n";
    msg << "first occurence on \n";
    return msg.str();
}
