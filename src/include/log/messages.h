#ifndef HAARD_LOG_MESSAGES_H
#define HAARD_LOG_MESSAGES_H

#include "defs.h"
#include "log/log.h"
#include "token/token.h"
#include "ast/ast.h"
#include "scope/symbol.h"

namespace haard {
    std::string get_line_for_message(std::string path, int line, int column, int count);
    Log* error_message_unexpected_token(std::string path, Token& token);
    Log* error_message_expected_token(std::string path, int kind, Token& token);
    Log* error_message_no_return_type(std::string path, Token& token);

    std::string error_header(std::string path, int line, int column);
}

#endif
