#include <iostream>
#include <cstring>
#include "ir/ir_builder.h"
#include "ir/ir_call.h"
#include "ir/ir_label.h"
#include "printer/ir_printer.h"

using namespace haard;

IRBuilder::IRBuilder() {
    logger = nullptr;
    current_module = nullptr;
    modules = new IRModules();
    ctx = nullptr;
}

IRBuilder::~IRBuilder() {

}

void IRBuilder::build(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        build_source(sources->get_source(i));
    }

    IRPrinter printer;
    printer.print_modules(modules);
    std::cout << printer.get_output();
}

void IRBuilder::build_source(Source* source) {
    IRModule* module = new IRModule();
    current_module = module;

    for (int i = 0; i < source->function_count(); ++i) {
        build_function(source->get_function(i));
    }

    current_module = nullptr;
    modules->add_module(module);
}

void IRBuilder::build_class(Class* klass) {

}

void IRBuilder::build_function(Function* function) {
    IRFunction* ir_func = new IRFunction();

    ir_func->set_name(function->get_qualified_name());

    ctx = ir_func->get_context();

    build_function_parameters(function, ir_func);
    build_function_body(function);
    ctx->move_allocas_to_instructions();

    current_module->add_function(ir_func);

    if (strcmp(function->get_name(), "main") == 0) {
        current_module->set_main_function(ir_func);
    }
}

void IRBuilder::build_function_parameters(Function* function, IRFunction* ir_func) {
    int size;
    int align;
    std::string name;
    Variable* var;

    for (int i = 0; i < function->parameters_count(); ++i) {
        IRValue* p = ctx->new_temporary();
        IRAlloca* alloca;

        ir_func->add_parameter(p);

        var = function->get_parameter(i);
        name = var->get_unique_name();
        size = var->get_type()->get_size_in_bytes();
        alloca = ctx->new_alloca(name, size);
        ctx->new_store(size, alloca->get_dst(), p);
    }
}

void IRBuilder::build_function_body(Function* function) {
    build_compound_statement(function->get_statements());
    ctx->new_unary(IR_RETURN, nullptr, nullptr);
}

void IRBuilder::set_logger(Logger* logger) {
    this->logger = logger;
}

IRModules* IRBuilder::get_modules() const {
    return modules;
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
}

void IRBuilder::build_expression_statement(ExpressionStatement* statement) {
    build_expression(statement->get_expression());
}

void IRBuilder::build_jump_statement(JumpStatement* statement) {

}

void IRBuilder::build_while_statement(WhileStatement* statement) {
    IRValue* cond;
    IRLabel* begin = ctx->new_label();
    IRLabel* after = ctx->new_label();

    IRValue* begin_label = ctx->new_label_value(begin->get_label());
    IRValue* after_label = ctx->new_label_value(after->get_label());

    ctx->add_instruction(begin);
    build_expression(statement->get_condition());
    cond = last_value;
    //ctx->new_bin(IR_BZ, nullptr, cond, after_label);
    ctx->new_branch(IR_BZ, cond, after_label);

    build_statement(statement->get_statements());
    //ctx->new_unary(IR_GOTO, nullptr, begin_label);
    ctx->new_branch(IR_GOTO, begin_label);
    ctx->add_instruction(after);
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

    //ctx->new_unary(IR_GOTO, nullptr, begin_label);
    ctx->new_branch(IR_GOTO, begin_label);
    ctx->add_instruction(after);
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
    alloca = ctx->new_alloca(name, size);
}

void IRBuilder::build_if(BranchStatement* statement) {
    IRValue* cond;
    IRLabel* fb = ctx->new_label();
    IRLabel* after = ctx->new_label();

    IRValue* fb_label = ctx->new_label_value(fb->get_label());
    IRValue* after_label = ctx->new_label_value(after->get_label());

    build_expression(statement->get_condition());
    cond = last_value;
    //ctx->new_bin(IR_BZ, nullptr, cond, fb_label);
    ctx->new_branch(IR_BZ, cond, fb_label);

    build_statement(statement->get_true_statements());
    //ctx->new_unary(IR_GOTO, nullptr, after_label);
    ctx->new_branch(IR_GOTO, after_label);
    ctx->add_instruction(fb);
    build_statement(statement->get_false_statements());
    ctx->add_instruction(after);
}

void IRBuilder::build_else(BranchStatement* statement) {
    build_statement(statement->get_true_statements());
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
    BinOp* bin = (BinOp*) expression;
    UnOp* un = (UnOp*) expression;
    Literal* literal = (Literal*) expression;
    CastExpression* cast = (CastExpression*) expression;
    ExpressionList* exprlist = (ExpressionList*) expression;

    switch (kind) {
    case EXPR_ID:
        build_identifier((Identifier*) expression, lvalue);
        break;

    case EXPR_PRE_INC:
        build_pre_inc(un);
        break;

    case EXPR_CALL:
        build_call(bin);
        break;

    case EXPR_INDEX:
        build_index_access(bin, lvalue);
        break;

    case EXPR_ASSIGN:
        build_assignment(bin, lvalue);
        break;

    case EXPR_LOGICAL_AND:
    case EXPR_LOGICAL_AND_OPER:
        build_logical_and(bin);
        break;

    case EXPR_EQ:
        build_equal(bin);
        break;

    case EXPR_NE:
        build_not_equal(bin);
        break;

    case EXPR_LT:
        build_less_than(bin);
        break;

    case EXPR_GT:
        build_greater_than(bin);
        break;

    case EXPR_LE:
        build_less_than_or_equal(bin);
        break;

    case EXPR_GE:
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

    case EXPR_ADDRESS_OF:
        build_address_of(un);
        break;

    case EXPR_DEREFERENCE:
        build_dereference(un, lvalue);
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

    case EXPR_LITERAL_SYMBOL:
        build_literal(literal, IR_VALUE_LITERAL_SYMBOL);
        break;

    case EXPR_CAST:
        build_cast(cast);
        break;

    case EXPR_PARENTHESIS:
        build_expression(un->get_expression(), lvalue);
        break;
    }
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

    type = id->get_type();
    size = type->get_size_in_bytes();

    if (type->is_primitive() || type->get_kind() == TYPE_POINTER || type->get_kind() == TYPE_ARRAY) {
        std::string name = id->get_unique_name();

        if (ctx->has_alloca(name)) {
            last_value = ctx->get_alloca_value(name);
        } else {
            alloca = ctx->new_alloca(name, size);
            last_value = alloca->get_dst();
        }
    }
}

void IRBuilder::build_identifier_rvalue(Identifier* id) {
    IRMemory* load;
    IRValue* tmp0;
    Type* type;
    int size;

    type = id->get_type();
    size = type->get_size_in_bytes();

    if (type->is_primitive() || type->get_kind() == TYPE_POINTER) {
        std::string name = id->get_unique_name();

        if (ctx->has_alloca(name)) {
            tmp0 = ctx->get_alloca_value(name);
            load = ctx->new_load(size, tmp0);
            last_value = load->get_dst();
        }
    } else if (type->get_kind() == TYPE_ARRAY) {
        std::string name = id->get_unique_name();

        if (ctx->has_alloca(name)) {
            last_value = ctx->get_alloca_value(name);
        }
    }
}

void IRBuilder::build_pre_inc(UnOp* un) {
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

void IRBuilder::build_cast(CastExpression* cast) {
    build_expression(cast->get_expression());
}

void IRBuilder::build_call(BinOp* bin) {
    IRCall* call = new IRCall();
    build_call_arguments(call, (ExpressionList*) bin->get_right());

    if (bin->get_left()->get_kind() == EXPR_ID) {
        Identifier* id = (Identifier*) bin->get_left();
        Function* f = (Function*) id->get_symbol()->get_descriptor(id->get_overloaded_index());
        std::string name = f->get_qualified_name();

        call->set_name(name);
        ctx->add_instruction(call);
    }

    if (bin->get_type()->get_kind() != TYPE_VOID) {
        call->set_dst(ctx->new_temporary());
        last_value = call->get_dst();
    }
}

void IRBuilder::build_call_arguments(IRCall* call, ExpressionList* args) {
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

void IRBuilder::build_index_access(BinOp* bin, bool lvalue) {
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
            std::cout << __FILE__ << "\nERROR\n";
            exit(0);
        }
    }
}

void IRBuilder::build_assignment(BinOp* bin, bool lvalue) {
    IRMemory* store;
    IRValue* left;
    IRValue* right;
    int size;

    build_expression(bin->get_right());
    right = last_value;

    build_expression(bin->get_left(), true);
    left = last_value;

    size = bin->get_left()->get_type()->get_size_in_bytes();

    // FIXME
    // on complex types, should call memcpy instead of a simple store
    store = ctx->new_store(size, left, right);
    last_value = left;
}

void IRBuilder::build_logical_and(BinOp* bin) {
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

void IRBuilder::build_equal(BinOp* bin) {
    build_binop(bin, IR_EQ);
}

void IRBuilder::build_not_equal(BinOp* bin) {
    build_binop(bin, IR_NE);
}

void IRBuilder::build_less_than(BinOp* bin) {
    bool f1 = bin->get_left()->get_type()->is_signed();
    bool f2 = bin->get_right()->get_type()->is_signed();

    if (f1 && f2) {
        build_binop(bin, IR_LT);
    } else {
        build_binop(bin, IR_ULT);
    }
}

void IRBuilder::build_greater_than(BinOp* bin) {
    bool f1 = bin->get_left()->get_type()->is_signed();
    bool f2 = bin->get_right()->get_type()->is_signed();

    if (f1 && f2) {
        build_binop(bin, IR_GT);
    } else {
        build_binop(bin, IR_UGT);
    }
}

void IRBuilder::build_less_than_or_equal(BinOp* bin) {
    bool f1 = bin->get_left()->get_type()->is_signed();
    bool f2 = bin->get_right()->get_type()->is_signed();

    if (f1 && f2) {
        build_binop(bin, IR_LE);
    } else {
        build_binop(bin, IR_ULE);
    }
}

void IRBuilder::build_greater_than_or_equal(BinOp* bin) {
    bool f1 = bin->get_left()->get_type()->is_signed();
    bool f2 = bin->get_right()->get_type()->is_signed();

    if (f1 && f2) {
        build_binop(bin, IR_GE);
    } else {
        build_binop(bin, IR_UGE);
    }
}

void IRBuilder::build_plus(BinOp* bin) {
    build_binop(bin, IR_ADD);
}

void IRBuilder::build_minus(BinOp* bin) {
    build_binop(bin, IR_SUB);
}

void IRBuilder::build_times(BinOp* bin) {
    build_binop(bin, IR_MUL);
}

void IRBuilder::build_division(BinOp* bin) {
    build_binop(bin, IR_DIV);
}

void IRBuilder::build_modulo(BinOp* bin) {
    build_binop(bin, IR_REM);
}

void IRBuilder::build_address_of(UnOp* op) {
    build_expression(op->get_expression(), true);
}

void IRBuilder::build_dereference(UnOp* op, bool lvalue) {
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

void IRBuilder::build_binop(BinOp* bin, int kind) {
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
    modules->add_string_literal(literal->get_lexeme());
}

