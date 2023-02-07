#include <sstream>
#include "log/info_messages.h"

using namespace haard;

std::string haard::get_info_header() {
    return "<blue>info: </blue>";
}

std::string haard::info_message_defining_source(Source* source) {
    std::stringstream msg;

    msg << get_info_header() << "defining entities for source file '" << source->get_path() << "'";
    return msg.str();
}

std::string haard::info_message_defining_class(Class* klass) {
    std::stringstream msg;

    int line = klass->get_line();
    int column = klass->get_column();
    std::string path = klass->get_path();
    std::string name = klass->get_qualified_name();
    msg << info_header(path, line, column) << "defining class " << name;
    return msg.str();
}

std::string haard::info_message_defining_function(Function* function) {
    std::stringstream msg;

    msg << get_info_header() << "defining function '" << function->get_type_signature() << "' on source file '" << function->get_source()->get_path();
    return msg.str();

}

std::string haard::info_message_defining_method(Function* method) {
    std::stringstream msg;

    msg << get_info_header() << "defining method '" << method->get_type_signature()
        << "' on class '" << method->get_class()->get_name() << "'";
    return msg.str();
}

std::string haard::info_header(std::string path, int line, int column) {
    std::stringstream ss;

    ss << path << ':' << line << ':' << column << ": <blue>info:</blue> ";
    return ss.str();
}

std::string info_message_define_enum(Enum* decl) {
    std::stringstream msg;

    int line = decl->get_line();
    int column = decl->get_column();
    std::string path = decl->get_full_filepath();
    std::string name = decl->get_qualified_name();
    msg << info_header(path, line, column) << "defining enum " << name;
    return msg.str();
}
