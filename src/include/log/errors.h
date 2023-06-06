#ifndef HAARD_LOG_ERRORS_H
#define HAARD_LOG_ERRORS_H

#include <string>
#include "token/token.h"

namespace haard {
    std::string error_unexpected_token(std::string path, Token& token);
}

#endif
