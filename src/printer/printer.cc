#include <iostream>
#include "printer/printer.h"
#include "defs.h"

using namespace haard;

Printer::Printer() {
    indent_c = 0;
}
 
std::string Printer::to_str() {
    return out.str();
}

void Printer::print_modules(Modules* modules) {
    for (int i = 0; i < modules->modules_count(); ++i) {
        print_module(modules->get_module(i));
    }
}

void Printer::print_module(Module* module) {
    if (module->import_count() > 0) {
        for (int i = 0; i < module->import_count(); ++i) {
            print_import(module->get_import(i));
            out << '\n';
        }

        out << '\n';
    }

    for (int i = 0; i < module->functions_count(); ++i) {
        print_function(module->get_function(i));
        out << '\n';
    }

    for (int i = 0;i < module->classes_count(); ++i) {
        print_class(module->get_class(i));
        out << '\n';
    }
}
            
void Printer::print_import(Import* import) {
    int i;

    out << "import ";

    if (import->path_count() > 0) {
        for (i = 0; i < import->path_count() - 1; ++i) {
            out << import->get_path(i) << '.';
        }

        out << import->get_path(i);
    }

    if (import->has_alias()) {
        out << " as " << import->get_alias();
    }
}

void Printer::print_class(Class* klass) {
    Variable* var;

    print_indentation();

    out << "class " << klass->get_name() << ":\n";
    indent();

    if (klass->fields_count() > 0) {
        for (int i = 0; i < klass->fields_count(); ++i) {
            print_indentation();
            var = klass->get_field(i);
            out << var->get_name() << " : ";
            print_type(var->get_type());
            out << '\n';        
        }

        out << '\n';
    }

    if (klass->methods_count() > 0) {
        for (int i = 0; i < klass->methods_count(); ++i) {
            print_function(klass->get_method(i));
            out << '\n';
        }

        out << '\n';
    }

    dedent();
}

void Printer::print_function(Function* function) {
    print_indentation();

    out << "def " << function->get_name();

    if (function->get_template_header()) {
        out << function->get_template_header()->to_str();
    }

    out << " : ";
    print_type(function->get_return_type());
    out << '\n';
    indent();

    print_parameters(function);
    print_compound_statement(function->get_statements());

    dedent();
}

void Printer::print_parameters(Function* function) {
    Variable* param = nullptr;

    if (function->parameters_count() > 0) {
        for (int i = 0; i < function->parameters_count(); ++i) {
            param = function->get_parameter(i);

            print_indentation();
            out << "@" << param->get_name() << " : ";
            print_type(param->get_type());
            out << "\n";
        }

        out << '\n';
    }
}

void Printer::print_type(Type* type) {
    int i;
    int kind = type->get_kind();
    IndirectionType* pr = (IndirectionType*) type;
    ArrayListType* al = (ArrayListType*) type;
    HashType* hs = (HashType*) type;
    TypeList* tl = (TypeList*) type;
    NamedType* named = (NamedType*) type;

    switch (kind) {
    case TYPE_U8:
    case TYPE_U16:
    case TYPE_U32:
    case TYPE_U64:
    case TYPE_I8:
    case TYPE_I16:
    case TYPE_I32:
    case TYPE_I64:
    case TYPE_VOID:
    case TYPE_SYMBOL:
    case TYPE_BOOL:
    case TYPE_UCHAR:
    case TYPE_CHAR:
    case TYPE_USHORT:
    case TYPE_SHORT:
    case TYPE_UINT:
    case TYPE_INT:
    case TYPE_ULONG:
    case TYPE_LONG:
    case TYPE_FLOAT:
    case TYPE_DOUBLE:
    case TYPE_STR:
    case TYPE_NAMED:
        out << type->to_str();
        break;

    case TYPE_ARRAY:
        print_type(al->get_subtype());

        out << "[";

        if (al->get_expression() != nullptr) {
            print_expression(al->get_expression());
        }

        out << "]";
        break;

    case TYPE_LIST:
        out << "[";
        print_type(al->get_subtype());
        out << "]";
        break;

    case TYPE_HASH:
        out << "{";
        print_type(hs->get_key_type());
        out << " : ";
        print_type(hs->get_value_type());
        out << "}";
        break;

    case TYPE_FUNCTION:
        print_type_list(" -> ", tl);
        break;

    case TYPE_TUPLE:
        print_type_list(", ", tl);
        break;

    case TYPE_POINTER:
        print_type(pr->get_subtype());
        out << "*";
        break;

    case TYPE_REFERENCE:
        print_type(pr->get_subtype());
        out << "&";
        break;

    case TYPE_PARENTHESIS:
        out << "(";
        print_type(pr->get_subtype());
        out << ")";

    default:
        break;
    }
}

void Printer::print_type_list(std::string oper, TypeList* tlist) {
    int i;

    for (i = 0; i < tlist->types_count() - 1; ++i) {
        print_type(tlist->get_type(i));
        out << oper;
    }

    print_type(tlist->get_type(i));
}

void Printer::print_statement(Statement* statement) {
    int kind = statement->get_kind();

    switch (kind) {
    case STMT_COMPOUND:
        print_compound_statement((CompoundStatement*) statement);
        break;

    case STMT_WHILE:
        print_while_statement((While*) statement);
        break;

    case STMT_FOR:
    case STMT_FOREACH:
        print_for_statement((ForStatement*) statement);
        break;

    case STMT_EXPRESSION:
        print_expression_statement((ExpressionStatement*) statement);
        break;

    case STMT_IF:
        print_if((If*) statement);
        break;

    case STMT_ELIF:
        print_elif((Elif*) statement);
        break;

    case STMT_ELSE:
        print_else((Else*) statement);
        break;

    case STMT_RETURN:
        print_jump_statement("return", (JumpStatement*) statement);
        break;

    case STMT_GOTO:
        print_jump_statement("goto", (JumpStatement*) statement);
        break;

    case STMT_YIELD:
        print_jump_statement("yield", (JumpStatement*) statement);
        break;

    case STMT_CONTINUE:
        print_jump_statement("continue", (JumpStatement*) statement);
        break;

    case STMT_BREAK:
        print_jump_statement("break", (JumpStatement*) statement);
        break;

    case STMT_VAR_DECL:
        print_variable_declaration((VarDeclaration*) statement);
        break;
    }
}

void Printer::print_if(If* stmt) {
    print_indentation();

    out << "if ";
    print_expression(stmt->get_condition());
    out << ":\n";
    indent();
    print_statement(stmt->get_true_statements());
    dedent();

    if (stmt->get_false_statements()) {
        print_statement(stmt->get_false_statements());
    }
}

void Printer::print_elif(Elif* stmt) {
    print_indentation();

    out << "elif ";
    print_expression(stmt->get_condition());
    out << ":\n";
    indent();
    print_statement(stmt->get_true_statements());
    dedent();

    if (stmt->get_false_statements()) {
        print_statement(stmt->get_false_statements());
    }
}

void Printer::print_else(Else* stmt) {
    print_indentation();

    out << "else:\n";
    indent();
    print_statement(stmt->get_statements());
    dedent();
}

void Printer::print_while_statement(While* statement) {
    print_indentation();
    out << "while ";
    print_expression(statement->get_condition());
    out << ":\n";
    indent();
    print_compound_statement(statement->get_statements());
    dedent();
    out << '\n';
}

void Printer::print_for_statement(ForStatement* statement) {
    print_indentation();
    out << "for ";

    if (statement->get_kind() == STMT_FOR) {
        //print_expression(statement->get_initialization());
        DBG; exit(0);
        out << "; ";
        print_expression(statement->get_condition());
        out << "; ";
        //print_expression(statement->get_increment());
        DBG; exit(0);
    } else {
        print_expression(statement->get_condition());
    }

    out << ":\n";
    indent();
    print_compound_statement(statement->get_statements());
    dedent();
    out << '\n';
}

void Printer::print_jump_statement(std::string op, JumpStatement* statement) {
    print_indentation();
    out << op;

    if (statement->get_expression() != nullptr) {
        out << ' ';
        print_expression(statement->get_expression());
    }

    out << '\n';
}

void Printer::print_compound_statement(CompoundStatement* statement) {
    for (int i = 0; i < statement->statements_count(); ++i) {
        print_statement(statement->get_statement(i));
    }
}

void Printer::print_expression_statement(ExpressionStatement* statement) {
    print_indentation();
    print_expression(statement->get_expression());
    out << "\n";
}

void Printer::print_variable_declaration(VarDeclaration* decl) {
    Variable* var;

    var = decl->get_variable();
    print_indentation();
    out << "var ";
    out << var->get_name();

    if (var->get_type() != nullptr) {
        out << " : ";
        print_type(var->get_type());
    }

    if (decl->get_expression() != nullptr) {
        out << " = ";
        print_expression(decl->get_expression());
    }

    out << '\n';
}

// You may wonder why so much boilerplate code. After all, most cases
// could be handled with a simple call to the virtual to_str() method.
//
// Well, the reason is because it helps me to keep track of what is already
// implemented and what is missing. Yeah, I know, not the best strategy, but
// at least keep me going on
void Printer::print_expression(Expression* expression) {
    if (expression == nullptr) return;

    int kind = expression->get_kind();
    Literal* literal = (Literal*) expression;
    ExpressionList* exprlist = (ExpressionList*) expression;

    switch (kind) {
    case EXPR_ID:
        print_identifier((Identifier*) expression);
        break;

    case EXPR_ASSIGNMENT:
        print_assignment((Assignment*) expression);
        break;

    case EXPR_SPECIAL_ASSIGNMENT:
        print_special_assignment((SpecialAssignment*) expression);
        break;

    case EXPR_SHIFT_RIGHT_LOGICAL_ASSIGNMENT:
        print_shift_right_logical_assignment((ShiftRightLogicalAssignment*) expression);
        break;

    case EXPR_SHIFT_RIGHT_ARITHMETIC_ASSIGNMENT:
        print_shift_right_arithmetic_assignment((ShiftRightArithmeticAssignment*) expression);
        break;

    case EXPR_SHIFT_LEFT_LOGICAL_ASSIGNMENT:
        print_shift_left_logical_assignment((ShiftLeftLogicalAssignment*) expression);
        break;

    case EXPR_TIMES_ASSIGNMENT:
        print_times_assignment((TimesAssignment*) expression);
        break;

    case EXPR_PLUS_ASSIGNMENT:
        print_plus_assignment((PlusAssignment*) expression);
        break;

    case EXPR_MODULO_ASSIGNMENT:
        print_modulo_assignment((ModuloAssignment*) expression);
        break;

    case EXPR_MINUS_ASSIGNMENT:
        print_minus_assignment((MinusAssignment*) expression);
        break;

    case EXPR_INTEGER_DIVISION_ASSIGNMENT:
        print_integer_division_assignment((IntegerDivisionAssignment*) expression);
        break;

    case EXPR_DIVISION_ASSIGNMENT:
        print_division_assignment((DivisionAssignment*) expression);
        break;

    case EXPR_BITWISE_NOT_ASSIGNMENT:
        print_bitwise_not_assignment((BitwiseNotAssignment*) expression);
        break;

    case EXPR_BITWISE_OR_ASSIGNMENT:
        print_bitwise_or_assignment((BitwiseOrAssignment*) expression);
        break;

    case EXPR_BITWISE_XOR_ASSIGNMENT:
        print_bitwise_xor_assignment((BitwiseXorAssignment*) expression);
        break;

    case EXPR_BITWISE_AND_ASSIGNMENT:
        print_bitwise_and_assignment((BitwiseAndAssignment*) expression);
        break;

    case EXPR_CAST:
        print_cast((Cast*) expression);
        break;

    case EXPR_LOGICAL_OR:
        print_logical_or((LogicalOr*) expression);
        break;

    case EXPR_LOGICAL_OR_OPER:
        print_logical_or_oper((LogicalOrOper*) expression);
        break;

    case EXPR_LOGICAL_AND:
        print_logical_and((LogicalAnd*) expression);
        break;

    case EXPR_LOGICAL_AND_OPER:
        print_logical_and_oper((LogicalAndOper*) expression);
        break;

    case EXPR_EQUAL:
        print_equal((Equal*) expression);
        break;

    case EXPR_NOT_EQUAL:
        print_not_equal((NotEqual*) expression);
        break;

    case EXPR_LESS_THAN:
        print_less_than((LessThan*) expression);
        break;

    case EXPR_GREATER_THAN:
        print_greater_than((GreaterThan*) expression);
        break;

    case EXPR_LESS_THAN_OR_EQUAL:
        print_less_than_or_equal((LessThanOrEqual*) expression);
        break;

    case EXPR_GREATER_THAN_OR_EQUAL:
        print_greater_than_or_equal((GreaterThanOrEqual*) expression);
        break;

    case EXPR_IN:
        print_in((In*) expression);
        break;

    case EXPR_NOT_IN:
        print_not_in((NotIn*) expression);
        break;

    case EXPR_INCLUSIVE_RANGE:
        print_inclusive_range((InclusiveRange*) expression);
        break;

    case EXPR_EXCLUSIVE_RANGE:
        print_exclusive_range((ExclusiveRange*) expression);
        break;

    case EXPR_PLUS:
        print_plus((Plus*) expression);
        break;

    case EXPR_MINUS:
        print_minus((Minus*) expression);
        break;

    case EXPR_TIMES:
        print_times((Times*) expression);
        break;

    case EXPR_DIVISION:
        print_division((Division*) expression);
        break;

    case EXPR_INTEGER_DIVISION:
        print_integer_division((IntegerDivision*) expression);
        break;

    case EXPR_MODULO:
        print_modulo((Modulo*) expression);
        break;

    case EXPR_POWER:
        print_power((Power*) expression);
        break;

    case EXPR_BITWISE_OR:
        print_bitwise_or((BitwiseOr*) expression);
        break;

    case EXPR_BITWISE_XOR:
        print_bitwise_xor((BitwiseXor*) expression);
        break;

    case EXPR_BITWISE_AND:
        print_bitwise_and((BitwiseAnd*) expression);
        break;

    case EXPR_SHIFT_LEFT_LOGICAL:
        print_shift_left_logical((ShiftLeftLogical*) expression);
        break;

    case EXPR_SHIFT_RIGHT_LOGICAL:
        print_shift_right_logical((ShiftRightLogical*) expression);
        break;

    case EXPR_SHIFT_RIGHT_ARITHMETIC:
        print_shift_right_arithmetic((ShiftRightArithmetic*) expression);
        break;

    case EXPR_LOGICAL_NOT_OPER:
        print_logical_not_oper((LogicalNotOper*) expression);
        break;

    case EXPR_LOGICAL_NOT:
        print_logical_not((LogicalNot*) expression);
        break;

    case EXPR_ADDRESS_OF:
        print_address_of((AddressOf*) expression);
        break;

    case EXPR_DEREFERENCE:
        print_dereference((Dereference*) expression);
        break;

    case EXPR_BITWISE_NOT:
        print_bitwise_not((BitwiseNot*) expression);
        break;

    case EXPR_UNARY_MINUS:
        print_unary_minus((UnaryMinus*) expression);
        break;

    case EXPR_UNARY_PLUS:
        print_unary_plus((UnaryPlus*) expression);
        break;

    case EXPR_PRE_INCREMENT:
        print_pre_increment((PreIncrement*) expression);
        break;

    case EXPR_PRE_DECREMENT:
        print_pre_decrement((PreDecrement*) expression);
        break;

    case EXPR_POS_INCREMENT:
        print_pos_increment((PosIncrement*) expression);
        break;

    case EXPR_POS_DECREMENT:
        print_pos_decrement((PosDecrement*) expression);
        break;

    case EXPR_SIZEOF:
        print_sizeof((Sizeof*) expression);
        break;

    case EXPR_PARENTHESIS:
        print_parenthesis((Parenthesis*) expression);
        break;

    case EXPR_CALL:
        print_call((Call*) expression);
        break;

    case EXPR_INDEX:
        print_index((Index*) expression);
        break;

    case EXPR_ARROW:
        print_arrow((Arrow*) expression);
        break;

    case EXPR_DOT:
        print_dot((Dot*) expression);
        break;

    case EXPR_LITERAL_BOOL:
    case EXPR_LITERAL_INTEGER:
    case EXPR_LITERAL_FLOAT:
    case EXPR_LITERAL_DOUBLE:
    case EXPR_LITERAL_CHAR:
    case EXPR_LITERAL_STRING:
    case EXPR_LITERAL_SYMBOL:
        print_literal(literal);
        break;

    case EXPR_LITERAL_NULL:
        out << "null";
        break;

    case EXPR_ARRAY:
        print_array((Array*) expression);
        break;

    case EXPR_LIST:
        print_list((List*) expression);
        break;

    case EXPR_TUPLE:
        print_tuple((Tuple*) expression);
        break;

    case EXPR_FOR_INIT:
    case EXPR_FOR_INC:
        print_expression_list("", "", exprlist);
        break;

    case EXPR_HASH:
        print_hash((Hash*) expression);
        break;

    case EXPR_HASH_PAIR:
        print_hash_pair((HashPair*) expression);
        break;

    case EXPR_FUNCTION:
        print_function_expression((FunctionExpression*) expression);
        break;

    case EXPR_DELETE:
        print_delete((Delete*) expression);
        break;

    case EXPR_DELETE_ARRAY:
        print_delete_array((DeleteArray*) expression);
        break;

    case EXPR_NEW:
        print_new_expression((New*) expression);
        break;
    }
}

void Printer::print_identifier(Identifier* id) {
    out << id->to_str();
}

void Printer::print_literal(Literal* literal) {
    out << literal->get_lexeme();
}

void Printer::print_expression_list(std::string begin, std::string end, ExpressionList* exprs) {
    int i;
    out << begin;

    if (exprs->expressions_count() > 0) {
        for (i = 0; i < exprs->expressions_count() - 1; ++i) {
            print_expression(exprs->get_expression(i));
            out << ", ";
        }

        print_expression(exprs->get_expression(i));
    }

    out << end;
}

void Printer::print_function_expression(FunctionExpression* function) {
    int i;
    Function* f = function->get_function();

    out << "|";

    if (f->parameters_count() > 0) {
        for (i = 0; i < f->parameters_count() - 1; ++i) {
            out << f->get_parameter(i)->get_name();
            out << ", ";
        }

        out << f->get_parameter(i)->get_name();
    }

    out << "| {\n";

    indent();
    print_compound_statement(f->get_statements());
    dedent();

    print_indentation();
    out << "}";
}

void Printer::print_new_expression(New* expr) {
    out << "new ";
    print_type(expr->get_new_type());

    if (expr->has_arguments()) {
        print_expression_list("(", ")", expr->get_arguments());
    }
}

void Printer::print_assignment(Assignment* expr) {
    print_binary_operator("=", expr);
}

void Printer::print_special_assignment(SpecialAssignment* expr) {
    print_binary_operator(":=", expr);
}

void Printer::print_shift_right_logical_assignment(ShiftRightLogicalAssignment* expr) {
    print_binary_operator(">>>=", expr);
}

void Printer::print_shift_right_arithmetic_assignment(ShiftRightArithmeticAssignment* expr) {
    print_binary_operator(">>=", expr);
}

void Printer::print_shift_left_logical_assignment(ShiftLeftLogicalAssignment* expr) {
    print_binary_operator("<<=", expr);
}

void Printer::print_times_assignment(TimesAssignment* expr) {
    print_binary_operator("*=", expr);
}

void Printer::print_plus_assignment(PlusAssignment* expr) {
    print_binary_operator("+=", expr);
}

void Printer::print_modulo_assignment(ModuloAssignment* expr) {
    print_binary_operator("%=", expr);
}

void Printer::print_minus_assignment(MinusAssignment* expr) {
    print_binary_operator("-=", expr);
}

void Printer::print_integer_division_assignment(IntegerDivisionAssignment* expr) {
    print_binary_operator("//=", expr);
}

void Printer::print_division_assignment(DivisionAssignment* expr) {
    print_binary_operator("/=", expr);
}

void Printer::print_bitwise_not_assignment(BitwiseNotAssignment* expr) {
    print_binary_operator("~=", expr);
}

void Printer::print_bitwise_or_assignment(BitwiseOrAssignment* expr) {
    print_binary_operator("|=", expr);
}

void Printer::print_bitwise_xor_assignment(BitwiseXorAssignment* expr) {
    print_binary_operator("^=", expr);
}

void Printer::print_bitwise_and_assignment(BitwiseAndAssignment* expr) {
    print_binary_operator("&=", expr);
}

void Printer::print_cast(Cast* expr) {
    print_expression(expr->get_expression());
    out << " as ";
    print_type(expr->get_cast_type());
}

void Printer::print_logical_or(LogicalOr* expr) {
    print_binary_operator("or", expr);
}

void Printer::print_logical_or_oper(LogicalOrOper* expr) {
    print_binary_operator("||", expr);
}

void Printer::print_logical_and(LogicalAnd* expr) {
    print_binary_operator("and", expr);
}

void Printer::print_logical_and_oper(LogicalAndOper* expr) {
    print_binary_operator("&&", expr);
}

void Printer::print_equal(Equal* expr) {
    print_binary_operator("==", expr);
}

void Printer::print_not_equal(NotEqual* expr) {
    print_binary_operator("!=", expr);
}

void Printer::print_less_than(LessThan* expr) {
    print_binary_operator("<", expr);
}

void Printer::print_greater_than(GreaterThan* expr) {
    print_binary_operator(">", expr);
}

void Printer::print_less_than_or_equal(LessThanOrEqual* expr) {
    print_binary_operator("<=", expr);
}

void Printer::print_greater_than_or_equal(GreaterThanOrEqual* expr) {
    print_binary_operator(">=", expr);
}

void Printer::print_in(In* expr) {
    print_binary_operator("in", expr);
}

void Printer::print_not_in(NotIn* expr) {
    print_binary_operator("not in", expr);
}

void Printer::print_inclusive_range(InclusiveRange* expr) {
    print_binary_operator("..", expr);
}

void Printer::print_exclusive_range(ExclusiveRange* expr) {
    print_binary_operator("...", expr);
}

void Printer::print_plus(Plus* expr) {
    print_binary_operator("+", expr);
}

void Printer::print_minus(Minus* expr) {
    print_binary_operator("-", expr);
}

void Printer::print_times(Times* expr) {
    print_binary_operator("*", expr);
}

void Printer::print_division(Division* expr) {
    print_binary_operator("/", expr);
}

void Printer::print_integer_division(IntegerDivision* expr) {
    print_binary_operator("//", expr);
}

void Printer::print_modulo(Modulo* expr) {
    print_binary_operator("%", expr);
}

void Printer::print_power(Power* expr) {
    print_binary_operator("**", expr);
}

void Printer::print_bitwise_or(BitwiseOr* expr) {
    print_binary_operator("|", expr);
}

void Printer::print_bitwise_xor(BitwiseXor* expr) {
    print_binary_operator("^", expr);
}

void Printer::print_bitwise_and(BitwiseAnd* expr) {
    print_binary_operator("&", expr);
}

void Printer::print_shift_left_logical(ShiftLeftLogical* expr) {
    print_binary_operator("<<", expr);
}

void Printer::print_shift_right_logical(ShiftRightLogical* expr) {
    print_binary_operator(">>>", expr);
}

void Printer::print_shift_right_arithmetic(ShiftRightArithmetic* expr) {
    print_binary_operator(">>", expr);
}

void Printer::print_logical_not_oper(LogicalNotOper* expr) {
    print_unary_operator("!", expr);
}

void Printer::print_logical_not(LogicalNot* expr) {
    print_unary_operator("not ", expr);
}

void Printer::print_address_of(AddressOf* expr) {
    print_unary_operator("&", expr);
}

void Printer::print_dereference(Dereference* expr) {
    print_unary_operator("*", expr);
}

void Printer::print_bitwise_not(BitwiseNot* expr) {
    print_unary_operator("~", expr);
}

void Printer::print_unary_minus(UnaryMinus* expr) {
    print_unary_operator("-", expr);
}

void Printer::print_unary_plus(UnaryPlus* expr) {
    print_unary_operator("+", expr);
}

void Printer::print_pre_increment(PreIncrement* expr) {
    print_unary_operator("++", expr);
}

void Printer::print_pre_decrement(PreDecrement* expr) {
    print_unary_operator("--", expr);
}

void Printer::print_pos_increment(PosIncrement* expr) {
    print_unary_operator("++", expr, true);
}

void Printer::print_pos_decrement(PosDecrement* expr) {
    print_unary_operator("--", expr, true);
}

void Printer::print_parenthesis(Parenthesis* expr) {
    out << "(";
    print_expression(expr->get_expression());
    out << ")";
}

void Printer::print_sizeof(Sizeof* expr) {
    out << "sizeof(" << expr->get_expression() << ")";
}

void Printer::print_call(Call* expr) {
    print_expression(expr->get_object());

    out << "(";

    if (expr->get_arguments()) {
        print_expression_list("", "", expr->get_arguments());
    }

    out << ")";
}

void Printer::print_index(Index* expr) {
    out << expr->get_object() << "[";

    if (expr->get_index()) {
        print_expression(expr->get_index());
    }

    out << "]";
}

void Printer::print_arrow(Arrow* expr) {
    print_binary_operator("->", expr, true);
}

void Printer::print_dot(Dot* expr) {
    print_binary_operator(".", expr, true);
}

void Printer::print_delete(Delete* expr) {
    out << "delete ";
    print_expression(expr->get_expression());
}

void Printer::print_delete_array(DeleteArray* expr) {
    out << "delete[] ";
    print_expression(expr->get_expression());
}

void Printer::print_array(Array* expr) {
    print_expression_list("{", "}", expr->get_expressions());
}

void Printer::print_list(List* expr) {
    print_expression_list("[", "]", expr->get_expressions());
}

void Printer::print_tuple(Tuple* expr) {
   print_expression_list("(", ")", expr->get_expressions());
}

void Printer::print_hash(Hash* expr) {
    print_expression_list("{", "}", expr->get_expressions());
}

void Printer::print_hash_pair(HashPair* expr) {
    print_expression(expr->get_left());
    out << ": ";
    print_expression(expr->get_right());
}

void Printer::print_binary_operator(const char* oper, BinaryOperator* expr, bool no_spaces) {
    print_expression(expr->get_left());

    if (no_spaces) {
        out << oper;
    } else {
        out << " " << oper << " ";
    }

    print_expression(expr->get_right());
}

void Printer::print_unary_operator(const char* oper, UnaryOperator* expr, bool after) {
    if (after) {
        print_expression(expr->get_expression());
        out << oper;
    } else {
        out << oper;
        print_expression(expr->get_expression());
    }
}

void Printer::indent() {
    indent_c++;
}

void Printer::dedent() {
    indent_c--;
}

void Printer::print_indentation() {
    for (int i = 0; i < indent_c; ++i) {
        out << "    ";
    }
}
