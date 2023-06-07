#ifndef HAARD_LOG_ERRORS_H
#define HAARD_LOG_ERRORS_H

#include <string>
#include "token/token.h"
#include "ast/ast.h"

namespace haard {
    std::string error_unexpected_token(std::string path, Token& token);
    std::string error_expected_elif_expression(std::string path, Elif* stmt);
    std::string error_expected_while_expression(std::string path, While* stmt);
}

#endif
