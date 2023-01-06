#include "vm/irvm_context.h"

using namespace haard;

IrVMContext::IrVMContext(uint64_t ip, uint64_t sp, uint64_t fp) {
    this->ip = ip;
    this->sp = sp;
    this->fp = fp;
}

uint64_t IrVMContext::get_ip() const {
    return ip;
}

void IrVMContext::set_ip(const uint64_t& value) {
    ip = value;
}

uint64_t IrVMContext::get_sp() const {
    return sp;
}

void IrVMContext::set_sp(const uint64_t& value) {
    sp = value;
}

uint64_t IrVMContext::get_fp() const {
    return fp;
}

void IrVMContext::set_fp(const uint64_t& value) {
    fp = value;
}

std::map<std::string, uint64_t> IrVMContext::get_values() const {
    return values;
}

void IrVMContext::set_values(const std::map<std::string, uint64_t>& value) {
    values = value;
}
