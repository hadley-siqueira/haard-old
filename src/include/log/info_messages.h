#ifndef HAARD_LOG_INFO_MESSAGES_H
#define HAARD_LOG_INFO_MESSAGES_H

#include <string>
#include "defs.h"
#include "log/log.h"
#include "token/token.h"
#include "ast/ast.h"

namespace haard {
    std::string get_info_header();
    std::string info_header(std::string path, int line, int column);
    std::string info_message_defining_source(Source* source);

    std::string info_message_defining_class(Class* klass);


    std::string info_message_defining_method(Function* method);

    std::string info_message_define_type(TypeDeclaration* decl, std::string kind);
    std::string info_message_define_function(Function* function);
}

#endif
