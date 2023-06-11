#ifndef HAARD_LOG_INFOS_H
#define HAARD_LOG_INFOS_H

#include <string>
#include "token/token.h"
#include "ast/ast.h"

namespace haard {
    std::string info_define_type(CompoundTypeDescriptor* decl, std::string msg);
}

#endif
