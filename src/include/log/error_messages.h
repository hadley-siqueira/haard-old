#ifndef HAARD_LOG_ERROR_MESSAGES_H
#define HAARD_LOG_ERROR_MESSAGES_H

#include <string>
#include "defs.h"
#include "log/log.h"
#include "token/token.h"
#include "ast/ast.h"
#include "scope/symbol.h"

namespace haard {
    std::string error_message_class_aready_defined(Class* klass, Symbol* sym);
}

#endif
