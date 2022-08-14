#ifndef HAARD_LOG_INFO_MESSAGES_H
#define HAARD_LOG_INFO_MESSAGES_H

#include <string>
#include "defs.h"
#include "log/log.h"
#include "token/token.h"
#include "ast/ast.h"

namespace haard {
    std::string info_message_defining_source(Source* source);
}

#endif
