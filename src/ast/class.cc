#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include "ast/class.h"
#include "scope/scope.h"
#include "ast/annotation.h"

using namespace haard;

int class_counter = 0;

Class::Class() {
    set_kind(DECL_CLASS);
    is_virtual_flag = false;
    uid = class_counter++;
}

Class::~Class() {

}

void Class::set_from_token(Token& token) {
    set_name(token.get_lexeme());
    set_line(token.get_line());
    set_column(token.get_column());
}

void Class::calculate_variables_offset() {
    int size = 0;
    int offset = 0;
    int align = 0;
    int max_align = 0;
    Variable* var;

    // if has a super class, the super class already calculated vtable pointer etc
    // if doesn't have super class, need to check if need vtable ptr
    if (get_super_type()) {
        Class* parent = (Class*) get_super_descriptor();
        offset = super_type->get_size_in_bytes() - parent->get_remaining_pad();
        max_align = super_type->get_alignment();
    } else {
        if (is_virtual()) {
            offset = ARCH_WORD_SIZE;
            max_align = ARCH_WORD_SIZE;
        }
    }

    for (int i = 0; i < fields_count(); ++i) {
        var = get_field(i);
        size = var->get_type()->get_size_in_bytes();
        align = var->get_type()->get_alignment();

        if (align > max_align) {
            max_align = align;
        }

        while (offset % align != 0) {
            ++offset;
        }

        var->set_offset(offset);
        offset += size;
    }

    // the remaining pad is used to calculate children class offsets
    // Maybe a child class can use the space left by the parent to
    // store one of its variables
    remaining_pad = 0;

    while (max_align != 0 && offset % max_align != 0) {
        ++offset;
        ++remaining_pad;
    }

    size_in_bytes = offset;
    alignment = max_align;
}

Function* Class::get_constructor(int idx) {
    if (idx < constructors_count()) {
        return constructors[idx];
    }

    return nullptr;
}

void Class::set_virtual(bool flag) {
    is_virtual_flag = flag;
}

bool Class::is_virtual() {
    return is_virtual_flag;
}

int Class::get_remaining_pad() const {
    return remaining_pad;
}

void Class::set_remaining_pad(int value) {
    remaining_pad = value;
}

int Class::get_size_in_bytes() {
    if (size_in_bytes < 0) {
        calculate_variables_offset();
    }

    return size_in_bytes;
}

std::string Class::get_qualified_name() {
    int i;
    std::stringstream ss;

    ss << get_module()->get_relative_path() << "." << get_name();

    if (get_template_header() && get_template_header()->types_count() > 0) {
        ss << "<";

        if (is_template()) {
            ss << get_template_header()->types_count();
        } else {
            for (i = 0; i < get_template_header()->types_count() - 1; ++i) {
                ss << get_template_header()->get_type(i)->get_qualified_name() << ", ";
            }

            ss << get_template_header()->get_type(i)->get_qualified_name();
        }

        ss << ">";
    }

    return ss.str();
}
