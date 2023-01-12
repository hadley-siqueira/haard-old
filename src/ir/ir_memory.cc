#include "ir/ir_memory.h"

using namespace haard;

IRMemory::IRMemory(int kind, IRValue *dst, IRValue *src, int offset) {
    set_kind(kind);
    set_dst(dst);
    set_src(src);
    set_offset(offset);
}

IRMemory::~IRMemory() {

}

IRValue* IRMemory::get_dst() const {
    return dst;
}

void IRMemory::set_dst(IRValue *value) {
    dst = value;
}

IRValue *IRMemory::get_src() const {
    return src;
}

void IRMemory::set_src(IRValue *value) {
    src = value;
}

int IRMemory::get_offset() const {
    return offset;
}

void IRMemory::set_offset(int value) {
    offset = value;
}
