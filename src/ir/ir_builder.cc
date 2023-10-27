#include <iostream>
#include <cstring>
#include "ir/ir_builder.h"
#include "ir/ir_call.h"
#include "ir/ir_label.h"
#include "printer/ir_printer.h"

using namespace haard;

IRBuilder::IRBuilder() {
    logger = nullptr;
    current_ir_module = nullptr;
    ir_modules = new IRModules();
    ctx = nullptr;
}

IRBuilder::~IRBuilder() {

}

void IRBuilder::build(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        build_module(modules->get_module(i));
    }
}

void IRBuilder::build_module(Module* module) {
    IRModule* ir_module = new IRModule();
    current_ir_module = ir_module;

    for (int i = 0; i < module->functions_count(); ++i) {
        build_function(module->get_function(i));
    }

    for (int i = 0; i < module->classes_count(); ++i) {
        build_class(module->get_class(i));
    }

    current_ir_module = nullptr;
    ir_modules->add_module(ir_module);
}

void IRBuilder::build_class(Class* klass) {
    for (int i = 0; i < klass->methods_count(); ++i) {
        build_function(klass->get_method(i));
    }
}

void IRBuilder::build_function(Function* function) {
    if (function->is_template()) {
        return;
    }

    IRFunction* ir_func = new IRFunction();

    ir_func->set_name(function->get_qualified_name());

    ctx = ir_func->get_context();

    build_function_parameters(function, ir_func);
    build_function_body(function);
    ctx->move_allocas_to_instructions();

    current_ir_module->add_function(ir_func);

    if (function->get_name() == "main") {
        current_ir_module->set_main_function(ir_func);
    }
}

void IRBuilder::build_function_parameters(Function* function, IRFunction* ir_func) {
    int size;
    int align;
    std::string name;
    Variable* var;

    // add this hidden parameter
    if (function->is_method()) {
        IRValue* p = ctx->new_temporary();
        IRAlloca* alloca;

        ir_func->add_parameter(p);

        name = "this";
        size = ARCH_WORD_SIZE;
        align = ARCH_WORD_SIZE;
        alloca = ctx->new_alloca(name, size, align);
        ctx->new_store(size, alloca->get_dst(), p);
    }

    for (int i = 0; i < function->parameters_count(); ++i) {
        IRValue* p = ctx->new_temporary();
        IRAlloca* alloca;

        ir_func->add_parameter(p);

        var = function->get_parameter(i);
        name = var->get_unique_name();
        size = var->get_type()->get_size_in_bytes();
        align = var->get_type()->get_alignment();
        alloca = ctx->new_alloca(name, size, align);

        // handle parameters passed by value
        if (var->get_type()->get_kind() != TYPE_NAMED) {
            ctx->new_store(size, alloca->get_dst(), p);
        } else {
            ctx->new_memcpy(alloca->get_dst(), p, size);
        }
    }
}

void IRBuilder::build_function_body(Function* function) {
    build_compound_statement(function->get_statements());
    ctx->new_unary(IR_RETURN, nullptr, nullptr);

    generate_deletables(function->get_scope());
}

void IRBuilder::set_logger(Logger* logger) {
    this->logger = logger;
}

IRModules* IRBuilder::get_modules() const {
    return ir_modules;
}

void IRBuilder::build_statement(Statement* statement) {
    if (statement == nullptr) return;

    int kind = statement->get_kind();

    switch (kind) {
    case STMT_COMPOUND:
        build_compound_statement((CompoundStatement*) statement);
        break;

    case STMT_WHILE:
        build_while_statement((WhileStatement*) statement);
        break;

    case STMT_FOR:
        build_for_statement((ForStatement*) statement);
        break;

    case STMT_FOREACH:
        break;

    case STMT_EXPRESSION:
        build_expression_statement((ExpressionStatement*) statement);
        break;

    case STMT_IF:
    case STMT_ELIF:
        build_if((BranchStatement*) statement);
        break;

    case STMT_ELSE:
        build_else((BranchStatement*) statement);
        break;

    case STMT_RETURN:
        build_return_statement((JumpStatement*) statement);
        break;

    case STMT_GOTO:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_YIELD:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_CONTINUE:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_BREAK:
        build_jump_statement((JumpStatement*) statement);
        break;

    case STMT_VAR_DECL:
        build_variable_declaration((VarDeclaration*) statement);
        break;
    }
}

void IRBuilder::build_compound_statement(CompoundStatement* stmts) {
    for (int i = 0; i < stmts->statements_count(); ++i) {
        build_statement(stmts->get_statement(i));
    }

    generate_deletables(stmts->get_scope());
}

void IRBuilder::build_expression_statement(ExpressionStatement* statement) {
    build_expression(statement->get_expression());
}

void IRBuilder::build_jump_statement(JumpStatement* statement) {

}

void IRBuilder::build_while_statement(WhileStatement *statement) {
    IRValue* cond;
    IRLabel* begin = ctx->new_label();
    IRLabel* after = ctx->new_label();

    IRValue* begin_label = ctx->new_label_value(begin->get_label());
    IRValue* after_label = ctx->new_label_value(after->get_label());

    ctx->add_instruction(begin);
    build_expression(statement->get_condition());
    cond = last_value;
    ctx->new_branch(IR_BZ, cond, after_label);

    build_statement(statement->get_statements());

    ctx->new_branch(IR_GOTO, begin_label);
    ctx->add_instruction(after);

    generate_deletables(statement->get_scope());
}

void IRBuilder::build_for_statement(ForStatement* statement) {
    IRValue* cond;
    IRLabel* begin = ctx->new_label();
    IRLabel* after = ctx->new_label();

    IRValue* begin_label = ctx->new_label_value(begin->get_label());
    IRValue* after_label = ctx->new_label_value(after->get_label());

    build_for_init(statement);

    ctx->add_instruction(begin);
    build_expression(statement->get_condition());

    cond = last_value;
    //ctx->new_bin(IR_BZ, nullptr, cond, after_label);
    ctx->new_branch(IR_BZ, cond, after_label);

    build_statement(statement->get_statements());
    build_for_inc(statement);

    ctx->new_branch(IR_GOTO, begin_label);
    ctx->add_instruction(after);

    generate_deletables(statement->get_scope());
}

void IRBuilder::build_for_init(ForStatement* statement) {
    ExpressionList* exprs = statement->get_initialization();

    for (int i = 0; i < exprs->expressions_count(); ++i) {
        build_expression(exprs->get_expression(i));
    }
}

void IRBuilder::build_for_inc(ForStatement* statement) {
    ExpressionList* exprs = statement->get_increment();

    for (int i = 0; i < exprs->expressions_count(); ++i) {
        build_expression(exprs->get_expression(i));
    }
}

void IRBuilder::build_branch_statement(BranchStatement* statement) {

}

void IRBuilder::build_variable_declaration(VarDeclaration* statement) {
    int size;
    int align;
    std::string name;
    Variable* var;
    IRAlloca* alloca;

    build_expression(statement->get_expression());

    var = statement->get_variable();
    name = var->get_unique_name();
    size = var->get_type()->get_size_in_bytes();
    align = var->get_type()->get_alignment();
    alloca = ctx->new_alloca(name, size, align);
}

void IRBuilder::build_if(BranchStatement* statement) {
    IRValue* cond;
    IRLabel* fb = ctx->new_label();
    IRLabel* after = ctx->new_label();

    IRValue* fb_label = ctx->new_label_value(fb->get_label());
    IRValue* after_label = ctx->new_label_value(after->get_label());

    build_expression(statement->get_condition());
    cond = last_value;
    ctx->new_branch(IR_BZ, cond, fb_label);

    build_statement(statement->get_true_statements());

    generate_deletables(statement->get_scope());
    ctx->new_branch(IR_GOTO, after_label);

    ctx->add_instruction(fb);
    build_statement(statement->get_false_statements());
    ctx->add_instruction(after);
}

void IRBuilder::build_else(BranchStatement* statement) {
    build_statement(statement->get_true_statements());
    generate_deletables(statement->get_scope());
}

void IRBuilder::build_return_statement(JumpStatement* statement) {
    if (statement->get_expression()) {
        build_expression(statement->get_expression());
        ctx->new_unary(IR_RETURN, nullptr, last_value);
    } else {
        ctx->new_unary(IR_RETURN, nullptr, nullptr);
    }
}

void IRBuilder::build_expression(Expression* expression, bool lvalue) {
    if (expression == nullptr) {
        return;
    }

    int kind = expression->get_kind();
    BinaryOperator* bin = (BinaryOperator*) expression;
    Literal* literal = (Literal*) expression;
    Cast* cast = (Cast*) expression;
    ExpressionList* exprlist = (ExpressionList*) expression;

    switch (kind) {
    case EXPR_THIS:
        build_this((This*) expression);
        break;

    case EXPR_ID:
        build_identifier((Identifier*) expression, lvalue);
        break;

    case EXPR_PRE_INCREMENT:
        build_pre_inc((PreIncrement*) expression);
        break;

    case EXPR_CALL:
        build_call((Call*) expression);
        break;

    case EXPR_INDEX:
        build_index_access(bin, lvalue);
        break;

    case EXPR_DOT:
        build_member_access(bin, lvalue);
        break;

    case EXPR_CONSTRUCTOR_ASSIGNMENT:
        build_constructor_assignment(bin);
        break;

    case EXPR_ASSIGNMENT:
        build_assignment(bin, lvalue);
        break;

    case EXPR_LOGICAL_OR:
    case EXPR_LOGICAL_OR_OPER:
        build_logical_or(bin);
        break;

    case EXPR_LOGICAL_AND:
    case EXPR_LOGICAL_AND_OPER:
        build_logical_and(bin);
        break;

    case EXPR_EQUAL:
        build_equal(bin);
        break;

    case EXPR_NOT_EQUAL:
        build_not_equal(bin);
        break;

    case EXPR_LESS_THAN:
        build_less_than((LessThan*) expression);
        break;

    case EXPR_GREATER_THAN:
        build_greater_than(bin);
        break;

    case EXPR_LESS_THAN_OR_EQUAL:
        build_less_than_or_equal(bin);
        break;

    case EXPR_GREATER_THAN_OR_EQUAL:
        build_greater_than_or_equal(bin);
        break;

    case EXPR_PLUS:
        build_plus(bin);
        break;

    case EXPR_MINUS:
        build_minus(bin);
        break;

    case EXPR_TIMES:
        build_times(bin);
        break;

    case EXPR_DIVISION:
        build_division(bin);
        break;

    case EXPR_MODULO:
        build_modulo(bin);
        break;

    case EXPR_BITWISE_OR:
        build_bitwise_or(bin);
        break;

    case EXPR_BITWISE_XOR:
        build_bitwise_xor(bin);
        break;

    case EXPR_BITWISE_AND:
        build_bitwise_and(bin);
        break;

    case EXPR_SHIFT_LEFT_LOGICAL:
        build_sll(bin);
        break;

    case EXPR_SHIFT_RIGHT_LOGICAL:
        build_srl(bin);
        break;

    case EXPR_SHIFT_RIGHT_ARITHMETIC:
        build_sra(bin);
        break;

    case EXPR_ADDRESS_OF:
        build_address_of((AddressOf*) expression);
        break;

    case EXPR_DEREFERENCE:
        build_dereference((Dereference*) expression, lvalue);
        break;

    case EXPR_LITERAL_BOOL:
        build_literal(literal, IR_VALUE_LITERAL_BOOL);
        break;

    case EXPR_LITERAL_INTEGER:
        build_literal(literal, IR_VALUE_LITERAL_INTEGER);
        break;

    case EXPR_LITERAL_FLOAT:
        build_literal(literal, IR_VALUE_LITERAL_FLOAT);
        break;

    case EXPR_LITERAL_DOUBLE:
        build_literal(literal, IR_VALUE_LITERAL_DOUBLE);
        break;

    case EXPR_LITERAL_CHAR:
        build_literal(literal, IR_VALUE_LITERAL_CHAR);
        break;

    case EXPR_LITERAL_STRING:
        build_literal_string(literal);
        break;

    case EXPR_STRING_BUILDER:
        build_string_builder((StringBuilder*) expression);
        break;

    case EXPR_LITERAL_SYMBOL:
        build_literal(literal, IR_VALUE_LITERAL_SYMBOL);
        break;

    case EXPR_CAST:
        build_cast(cast);
        break;

    case EXPR_PARENTHESIS:
        build_expression(((Parenthesis*) expression)->get_expression(), lvalue);
        break;

    case EXPR_SIZEOF:
        build_sizeof((Sizeof*) expression);
        break;
    }
}

void IRBuilder::build_scope(BinaryOperator* bin, bool lvalue) {
    Identifier* id = (Identifier*) bin->get_right();

    build_identifier(id, lvalue);
}

void IRBuilder::build_identifier(Identifier* id, bool lvalue) {
    if (lvalue) {
        build_identifier_lvalue(id);
    } else {
        build_identifier_rvalue(id);
    }
}

void IRBuilder::build_identifier_lvalue(Identifier* id) {
    IRAlloca* alloca;
    Type* type;
    int size;
    int align;
    //int kind;

    //kind = id->get_symbol_descriptor()->get_kind();

    if (id->is_local_variable()) {
        type = id->get_type();
        size = type->get_size_in_bytes();
        align = type->get_alignment();

        Variable* var = (Variable*) id->get_symbol_descriptor()->get_descriptor();
        std::string name = var->get_unique_name();

        if (ctx->has_alloca(name)) {
            last_value = ctx->get_alloca_value(name);
        } else {
            alloca = ctx->new_alloca(name, size, align);
            last_value = alloca->get_dst();
        }
    } else if (id->is_class_variable()) {
        IRValue* this_ptr = ctx->new_load(ARCH_WORD_SIZE, ctx->get_alloca_value("this"))->get_dst();
        Variable* var = (Variable*) id->get_symbol_descriptor()->get_descriptor();
        IRValue* offset = ctx->new_load_immediate(IR_VALUE_LITERAL_INTEGER, var->get_offset())->get_dst();
        IRValue* add = ctx->new_binary(IR_ADD, this_ptr, offset)->get_dst();
        last_value = add;
    }
}

void IRBuilder::build_identifier_rvalue(Identifier* id) {
    IRMemory* load;
    Type* type;
    int size;

    build_identifier_lvalue(id);
    type = id->get_type();
    size = type->get_size_in_bytes();

    if (type->is_integer_scalar()) {
        load = ctx->new_load(size, last_value);
        last_value = load->get_dst();
    }
}

void IRBuilder::build_this(This* expr) {
    last_value = ctx->get_alloca_value("this");
}

void IRBuilder::build_pre_inc(PreIncrement* un) {
    int size;
    IRValue* addr;
    IRValue* cst;
    IRMemory* load;
    IRBin* add;

    build_expression(un->get_expression(), true);
    size = un->get_expression()->get_type()->get_size_in_bytes();

    addr = last_value;
    load = ctx->new_load(size, addr);
    cst = ctx->new_load_immediate(IR_VALUE_LITERAL_INTEGER, "1")->get_dst();
    add  = ctx->new_binary(IR_ADD, load->get_dst(), cst);
    ctx->new_store(size, addr, add->get_dst());
    last_value = add->get_dst();
}

void IRBuilder::build_cast(Cast* cast) {
    build_expression(cast->get_expression());
}

void IRBuilder::build_call(Call* bin) {
    IRCall* call = new IRCall();

    /* refactor:
     * always call build_expression for left side:
     *      if is id and function or member, just grab the name
     *      if it is a dot
     *          if a member variable, returns its address and use as this
     *          if a method, returns name and last value (?)
     */

    if (is_function_call(bin)) {
        build_function_call(bin, call);
    }/* else if (is_method_call(bin)) {
        build_method_call(bin, call);
    } else if (is_constructor_call(bin)) {
        //DBG; exit(0);
    } else if (bin->get_left()->get_kind() == EXPR_DOT) {
        IRValue* this_ptr;

        build_expression(bin->get_left(), true);
        this_ptr = last_value;
        BinaryOperator* dot = (BinaryOperator*) bin->get_left();

        Identifier* id = (Identifier*) dot->get_right();
        Function* f = (Function*) id->get_symbol_descriptor()->get_descriptor();
        std::string name = f->get_qualified_name();

        call->set_name(name);

        build_call_arguments(call, (ExpressionList*) bin->get_right(), this_ptr);
        ctx->add_instruction(call);
    }*/

    if (bin->get_type()->get_kind() != TYPE_VOID) {
        call->set_dst(ctx->new_temporary());
        last_value = call->get_dst();
    }
}

void IRBuilder::build_call_arguments(IRCall* call, ExpressionList* args, IRValue* this_ptr) {
    if (this_ptr) {
        call->add_argument(this_ptr);
    }

    if (args == nullptr) {
        return;
    }

    if (args->expressions_count() > 0) {
        for (int i = 0; i < args->expressions_count(); ++i) {
            build_expression(args->get_expression(i));
            call->add_argument(last_value);
        }
    }
}

void IRBuilder::build_function_call(Call* bin, IRCall* call) {
    Identifier* id;

    if (bin->get_object()->get_kind() == EXPR_ID) {
        id = (Identifier*) bin->get_object();
    }

    Function* f = (Function*) id->get_symbol_descriptor()->get_descriptor();
    std::string name = f->get_qualified_name();

    call->set_name(name);

    build_call_arguments(call, (ExpressionList*) bin->get_arguments());
    ctx->add_instruction(call);
}

void IRBuilder::build_method_call(BinaryOperator* bin, IRCall* call) {
    Identifier* id = (Identifier*) bin->get_left();
    Function* f = (Function*) id->get_symbol_descriptor()->get_descriptor();
    std::string name = f->get_qualified_name();

    call->set_name(name);
    IRValue* this_ptr = ctx->new_load(ARCH_WORD_SIZE, ctx->get_alloca_value("this"))->get_dst();

    build_call_arguments(call, (ExpressionList*) bin->get_right(), this_ptr);
    ctx->add_instruction(call);
}

void IRBuilder::build_index_access(BinaryOperator* bin, bool lvalue) {
    std::stringstream ss;
    int size_in_bytes;
    IRValue* size;
    IRValue* base;
    IRValue* index;
    IRValue* addr;
    IRValue* offset;
    IRMemory* load;

    if (bin->get_left()->get_type()->get_kind() == TYPE_ARRAY) {
        build_expression(bin->get_left(), true);
    } else if (bin->get_left()->get_type()->get_kind() == TYPE_POINTER) {
        build_expression(bin->get_left());
    }

    base = last_value;
    build_expression(bin->get_right());
    offset = last_value;

    size_in_bytes = bin->get_type()->get_size_in_bytes();
    ss << size_in_bytes;

    size = ctx->new_load_immediate(IR_VALUE_LITERAL_INTEGER, ss.str())->get_dst();
    index = ctx->new_binary(IR_MUL, offset, size)->get_dst();
    addr = ctx->new_binary(IR_ADD, base, index)->get_dst();

    if (lvalue) {
        last_value = addr;
    } else {
        if (bin->get_type()->is_primitive()) {
            load = ctx->new_load(size_in_bytes, addr);
            last_value = load->get_dst();
        } else {
            DBG;
            exit(0);
        }
    }
}

void IRBuilder::build_member_access(BinaryOperator* bin, bool lvalue) {
    Identifier* id;
    Variable* var;

    build_expression(bin->get_left(), true);
    id = (Identifier*) bin->get_right();

    // if left side is a pointer, we want the value of the pointer
    // and not the address of the pointer itself
    if (bin->get_left()->get_type()->get_kind() == TYPE_POINTER) {
        last_value = ctx->new_load(ARCH_WORD_SIZE, last_value)->get_dst();
    }

    if (lvalue) {
        if (id->get_symbol_descriptor()->get_kind() == SYM_CLASS_VARIABLE) {
            IRValue* base = last_value;
            IRValue* offset;

            var = (Variable*) id->get_symbol_descriptor()->get_descriptor();;
            offset = ctx->new_load_immediate(IR_VALUE_LITERAL_INTEGER, var->get_offset())->get_dst();
            last_value = ctx->new_binary(IR_ADD, base, offset)->get_dst();
        }
    } else {
        if (id->get_symbol_descriptor()->get_kind() == SYM_CLASS_VARIABLE) {
            IRValue* base = last_value;
            IRValue* offset;

            var = (Variable*) id->get_symbol_descriptor()->get_descriptor();;
            offset = ctx->new_load_immediate(IR_VALUE_LITERAL_INTEGER, var->get_offset())->get_dst();
            last_value = ctx->new_binary(IR_ADD, base, offset)->get_dst();

            Type* type = id->get_type();
            int size = type->get_size_in_bytes();

            if (type->is_primitive() || type->get_kind() == TYPE_POINTER) {
                last_value = ctx->new_load(size, last_value)->get_dst();
            }
        }
    }
}

void IRBuilder::build_assignment(BinaryOperator* bin, bool lvalue) {
    IRMemory* store;
    IRValue* left;
    IRValue* right;
    Type* type;
    int size;

    build_expression(bin->get_right());
    right = last_value;

    build_expression(bin->get_left(), true);
    left = last_value;

    type = bin->get_left()->get_type();
    size = type->get_size_in_bytes();

    // FIXME
    // on complex types, should call memcpy instead of a simple store
    if (type->get_kind() != TYPE_NAMED) {
        store = ctx->new_store(size, left, right);
        last_value = left;
    } else {
        ctx->new_memcpy(left, right, size);
        last_value = left;
    }
}

void IRBuilder::build_constructor_assignment(BinaryOperator* bin) {
    IRValue* this_ptr;
    std::string name;
    Function* f;
    BinaryOperator* constructor;
    IRCall* call = new IRCall();

    build_expression(bin->get_left(), true);
    this_ptr = last_value;

    constructor = (BinaryOperator*) bin->get_right();
    int idx = 0;//constructor->get_overloaded_index();
    f = nullptr;//(Function*) constructor->get_constructor_call()->get_descriptor(idx);
    name = f->get_qualified_name();

    call->set_name(name);

    build_call_arguments(call, (ExpressionList*) constructor->get_right(), this_ptr);
    ctx->add_instruction(call);
}

void IRBuilder::build_logical_or(BinaryOperator* bin) {
    IRLabel* tb = ctx->new_label();
    IRLabel* af = ctx->new_label();
    IRValue* true_label = ctx->new_label_value(tb->get_label());
    IRValue* after_label = ctx->new_label_value(af->get_label());
    IRUnary* li;
    IRValue* alloca_addr;
    int size = 1;

    alloca_addr = ctx->new_tmp_alloca(size)->get_dst();

    build_expression(bin->get_left());
    ctx->new_branch(IR_BNZ, last_value, true_label);

    build_expression(bin->get_right());
    ctx->new_branch(IR_BNZ, last_value, true_label);

    li = ctx->new_load_immediate(IR_VALUE_LITERAL_BOOL, "false");
    ctx->new_store(size, alloca_addr, li->get_dst());
    ctx->new_branch(IR_GOTO, after_label);

    ctx->add_instruction(tb);
    li = ctx->new_load_immediate(IR_VALUE_LITERAL_BOOL, "true");
    ctx->new_store(size, alloca_addr, li->get_dst());

    ctx->add_instruction(af);
    last_value = ctx->new_load(size, alloca_addr)->get_dst();
}

void IRBuilder::build_logical_and(BinaryOperator* bin) {
    IRLabel* fb = ctx->new_label();
    IRLabel* af = ctx->new_label();
    IRValue* false_label = ctx->new_label_value(fb->get_label());
    IRValue* after_label = ctx->new_label_value(af->get_label());
    IRUnary* li;
    IRValue* alloca_addr;
    int size = 1;

    alloca_addr = ctx->new_tmp_alloca(size)->get_dst();
    build_expression(bin->get_left());
    ctx->new_branch(IR_BZ, last_value, false_label);

    build_expression(bin->get_right());
    ctx->new_branch(IR_BZ, last_value, false_label);
    li = ctx->new_load_immediate(IR_VALUE_LITERAL_BOOL, "true");
    ctx->new_store(size, alloca_addr, li->get_dst());
    ctx->new_branch(IR_GOTO, after_label);

    ctx->add_instruction(fb);
    li = ctx->new_load_immediate(IR_VALUE_LITERAL_BOOL, "false");
    ctx->new_store(size, alloca_addr, li->get_dst());

    ctx->add_instruction(af);
    last_value = ctx->new_load(size, alloca_addr)->get_dst();
}

void IRBuilder::build_equal(BinaryOperator* bin) {
    build_binop(bin, IR_EQ);
}

void IRBuilder::build_not_equal(BinaryOperator* bin) {
    build_binop(bin, IR_NE);
}

void IRBuilder::build_less_than(LessThan* bin) {
    bool f1 = bin->get_left()->get_type()->is_signed();
    bool f2 = bin->get_right()->get_type()->is_signed();

    if (f1 && f2) {
        build_binop(bin, IR_LT);
    } else {
        build_binop(bin, IR_ULT);
    }
}

void IRBuilder::build_greater_than(BinaryOperator* bin) {
    bool f1 = bin->get_left()->get_type()->is_signed();
    bool f2 = bin->get_right()->get_type()->is_signed();

    if (f1 && f2) {
        build_binop(bin, IR_GT);
    } else {
        build_binop(bin, IR_UGT);
    }
}

void IRBuilder::build_less_than_or_equal(BinaryOperator* bin) {
    bool f1 = bin->get_left()->get_type()->is_signed();
    bool f2 = bin->get_right()->get_type()->is_signed();

    if (f1 && f2) {
        build_binop(bin, IR_LE);
    } else {
        build_binop(bin, IR_ULE);
    }
}

void IRBuilder::build_greater_than_or_equal(BinaryOperator* bin) {
    bool f1 = bin->get_left()->get_type()->is_signed();
    bool f2 = bin->get_right()->get_type()->is_signed();

    if (f1 && f2) {
        build_binop(bin, IR_GE);
    } else {
        build_binop(bin, IR_UGE);
    }
}

void IRBuilder::build_plus(BinaryOperator* bin) {
    build_binop(bin, IR_ADD);
}

void IRBuilder::build_minus(BinaryOperator* bin) {
    build_binop(bin, IR_SUB);
}

void IRBuilder::build_times(BinaryOperator* bin) {
    build_binop(bin, IR_MUL);
}

void IRBuilder::build_division(BinaryOperator* bin) {
    build_binop(bin, IR_DIV);
}

void IRBuilder::build_modulo(BinaryOperator* bin) {
    build_binop(bin, IR_REM);
}

void IRBuilder::build_bitwise_or(BinaryOperator* bin) {
    build_binop(bin, IR_BITWISE_OR);
}

void IRBuilder::build_bitwise_xor(BinaryOperator* bin) {
    build_binop(bin, IR_BITWISE_XOR);
}

void IRBuilder::build_bitwise_and(BinaryOperator* bin) {
    build_binop(bin, IR_BITWISE_AND);
}

void IRBuilder::build_sll(BinaryOperator* bin) {
    build_binop(bin, IR_SLL);
}

void IRBuilder::build_srl(BinaryOperator* bin) {
    build_binop(bin, IR_SRL);
}

void IRBuilder::build_sra(BinaryOperator* bin) {
    if (bin->get_left()->get_type()->is_signed()) {
        build_binop(bin, IR_SRA);
    } else {
        build_binop(bin, IR_SRL);
    }
}

void IRBuilder::build_address_of(AddressOf* op) {
    build_expression(op->get_expression(), true);
}

void IRBuilder::build_dereference(Dereference *op, bool lvalue) {
    IRMemory* load;
    Type* type;
    int size;
    build_expression(op->get_expression());

    type = op->get_type();
    size = type->get_size_in_bytes();

    if (!lvalue) {
        load = ctx->new_load(size, last_value);
        last_value = load->get_dst();
    }
}

void IRBuilder::build_binop(BinaryOperator* bin, int kind) {
    IR* ir;
    IRValue* left;
    IRValue* right;
    IRValue* dst;

    build_expression(bin->get_left());
    left = last_value;

    build_expression(bin->get_right());
    right = last_value;

    dst = ctx->new_temporary();
    ir = ctx->new_bin(kind, dst, left, right);
    last_value = dst;
}

void IRBuilder::build_literal(Literal* literal, int kind) {
    IRUnary* li;

    li = ctx->new_load_immediate(kind, literal->get_lexeme());
    last_value = li->get_dst();
}

void IRBuilder::build_literal_integer(Literal* literal) {
    build_literal(literal, IR_VALUE_LITERAL_INTEGER);
}

void IRBuilder::build_literal_string(Literal* literal) {
    build_literal(literal, IR_VALUE_LITERAL_STRING);
    ir_modules->add_string_literal(literal->get_lexeme());
}

void IRBuilder::build_string_builder(StringBuilder* sb) {
    int size;
    int align;
    std::string name;
    Variable* var;
    IRAlloca* alloca;

    var = sb->get_variable();
    name = var->get_unique_name();
    size = var->get_type()->get_size_in_bytes();
    align = var->get_type()->get_alignment();
    alloca = ctx->new_alloca(name, size, align);

    for (int i = 0; i < sb->calls_count(); ++i) {
        build_expression(sb->get_call(i));
    }
}

void IRBuilder::build_sizeof(Sizeof* un) {
    Type* type = un->get_expression()->get_type();

    last_value = ctx->new_load_immediate(IR_VALUE_LITERAL_INTEGER, type->get_size_in_bytes())->get_dst();
}

void IRBuilder::generate_deletables(Scope* scope) {
    for (int i = 0; i < scope->deletables_count(); ++i) {
        build_expression(scope->get_deletable(i));
    }
}

bool IRBuilder::is_function_call(Call* bin) {
    Identifier* id;

    if (bin->get_object()->get_kind() == EXPR_ID) {
        id = (Identifier*) bin->get_object();
        return id->get_symbol_descriptor()->get_kind() == SYM_FUNCTION;
    }

    return false;
}

bool IRBuilder::is_method_call(BinaryOperator* bin) {
    Identifier* id;

    if (bin->get_left()->get_kind() == EXPR_ID) {
        id = (Identifier*) bin->get_left();
        return id->get_symbol_descriptor()->get_kind() == SYM_METHOD;
    }

    return false;
}

bool IRBuilder::is_constructor_call(BinaryOperator* bin) {
    Identifier* id;
    Type* type;
    Symbol* sym;

    if (bin->get_left()->get_kind() == EXPR_ID) {
        id = (Identifier*) bin->get_left();
        type = id->get_type();
        sym = nullptr;

        return type->get_kind() == TYPE_NAMED; //&& sym->get_kind() == SYM_CLASS;
    }

    return false;
}
