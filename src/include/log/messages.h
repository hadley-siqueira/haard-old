#ifndef HAARD_LOG_MESSAGES_H
#define HAARD_LOG_MESSAGES_H

#include "defs.h"
#include "log/log.h"
#include "token/token.h"

namespace haard {
    Log* error_message_unexpected_token(std::string path, Token& token);
}

#endif
