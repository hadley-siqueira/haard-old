#ifndef HAARD_LOG_ERROR_MESSAGES_H
#define HAARD_LOG_ERROR_MESSAGES_H

#include <string>
#include "defs.h"
#include "log/log.h"
#include "token/token.h"
#include "ast/ast.h"
#include "scope/symbol.h"

namespace haard {
    std::string error_message_cant_define_class(Class* klass, Symbol* sym);
    std::string error_message_id_not_in_scope(Module* source, Identifier* id);
}

#endif
