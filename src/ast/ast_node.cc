#include "ast/ast_node.h"

using namespace haard;

AstNode::AstNode() {
    line = 0;
    column = 0;
}

AstNode::~AstNode() {

}

int AstNode::get_kind() const {
    return kind;
}

void AstNode::set_kind(int value) {
    kind = value;
}

int AstNode::get_line() const {
    return line;
}

void AstNode::set_line(int value) {
    line = value;
}

int AstNode::get_column() const {
    return column;
}

void AstNode::set_column(int value) {
    column = value;
}
