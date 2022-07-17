#include "defs.h"
#include "ast/statement.h"

using namespace haard;

Statement::Statement() {
    kind = STMT_UNKNOWN;
}

Statement::~Statement() {

}

int Statement::get_kind() {
    return kind;
}

void Statement::set_kind(int kind) {
    this->kind = kind;
}
