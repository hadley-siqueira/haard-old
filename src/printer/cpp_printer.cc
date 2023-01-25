#include <iostream>
#include <cstring>
#include "printer/cpp_printer.h"
#include "defs.h"

using namespace haard;

CppPrinter::CppPrinter() {
    indent_c = 0;
    symbol_counter = 0;
    out = &res;
    main_function = nullptr;
}
 
std::string CppPrinter::to_str() {
    res << "#include <cstdint>\n";
    res << "#include <cstdio>\n";
    res << "#include <iostream>\n";
    res << "#include <string>\n";
    res << "#include <sstream>\n";
    res << "#include <vector>\n";
    res << "#include <map>\n\n";
    res << "typedef char* sym;\n";
    res << '\n';
    generate_symbols(res);
    res << classes_signatures.str();
    res << '\n';
    res << function_signatures.str();
    res << '\n';
    res << classes_bodies.str();
    res << '\n';
    res << function_bodies.str();
    res << '\n';

    out = &res;
    *out << "int main(int argc, char* argv[]) {\n";

    if (main_function) {
        *out << "    " << main_function->get_cpp_name() << "();\n";
    }

    *out << "}\n";

    return res.str();
}

void CppPrinter::print_sources(Sources* sources) {
    for (int i = 0; i < sources->sources_count(); ++i) {
        print_source(sources->get_source(i));
    }
}

void CppPrinter::print_source(Source* source) {
    /*if (source->import_count() > 0) {
        for (int i = 0; i < source->import_count(); ++i) {
            print_import(source->get_import(i));
            *out << '\n';
        }

        *out << '\n';
    }*/

    for (int i = 0;i < source->classes_count(); ++i) {
        print_class(source->get_class(i));
        *out << '\n';
    }

    for (int i = 0; i < source->function_count(); ++i) {
        print_function(source->get_function(i));
        *out << '\n';
    }
}
            
void CppPrinter::print_import(Import* import) {
    int i;

    *out << "import ";

    if (import->path_count() > 0) {
        for (i = 0; i < import->path_count() - 1; ++i) {
            *out << import->get_path(i) << '.';
        }

        *out << import->get_path(i);
    }

    if (import->has_alias()) {
        *out << " as " << import->get_alias();
    }
}

void CppPrinter::print_class(Class* klass) {
    Variable* var;

    out = &classes_bodies;
    print_indentation();

    *out << "class " << klass->get_cpp_name();

    if (klass->get_super_class()) {
        *out << " : public ";
        *out << klass->get_super_class()->to_cpp();
    }

    *out << " {\npublic:\n";
    classes_signatures << "class " << klass->get_cpp_name() << ";\n";
    indent();

    if (klass->variables_count() > 0) {
        for (int i = 0; i < klass->variables_count(); ++i) {
            print_indentation();
            var = klass->get_variable(i);
            print_type(var->get_type());
            *out << " cv" << var->get_uid() << "_" << var->get_name() << ";\n" ;
        }

        *out << '\n';
    }

    if (klass->methods_count() > 0) {
        for (int i = 0; i < klass->methods_count(); ++i) {
            print_function(klass->get_method(i));
            *out << '\n';
        }
    }

    print_class_constructors(klass);
    print_class_destructor(klass);

    *out << "};\n";
    dedent();
}

void CppPrinter::print_function(Function* function) {
    if (function->is_template()) return;

    if (function->is_method()) {
        out = &classes_bodies;
    } else {
        out = &function_bodies;
    }

    print_indentation();

    *out << function->get_cpp_signature();

    if (!function->is_method()) {
        function_signatures << function->get_cpp_signature() << ";\n";
    }

    *out << " {\n";

    if (function->get_name() == "main") {
        main_function = function;
    }

    indent();
    print_compound_statement(function->get_statements());

    dedent();
    print_indentation();
    *out << "}\n";
}

void CppPrinter::print_method(Function* function) {

}

void CppPrinter::print_parameters(Function* function) {
    Variable* param = nullptr;
    int i;

    *out << "(";

    if (function->parameters_count() > 0) {
        for (i = 0; i < function->parameters_count() - 1; ++i) {
            param = function->get_parameter(i);
            print_type(param->get_type());
            *out << ' ' << param->get_unique_name() << ", ";
        }

        param = function->get_parameter(i);
        print_type(param->get_type());
        *out << ' ' << param->get_unique_name();
    }

    *out << ") {\n";
}

void CppPrinter::print_type(Type* type) {
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
        *out << type->to_cpp();
        break;

    case TYPE_ARRAY:
        print_type(al->get_subtype());

        *out << "[";

        if (al->get_expression() != nullptr) {
            print_expression(al->get_expression());
        }

        *out << "]";
        break;

    case TYPE_LIST:
        *out << "[";
        print_type(al->get_subtype());
        *out << "]";
        break;

    case TYPE_HASH:
        *out << "{";
        print_type(hs->get_key_type());
        *out << " : ";
        print_type(hs->get_value_type());
        *out << "}";
        break;

    case TYPE_FUNCTION:
        print_type_list(" -> ", tl);
        break;

    case TYPE_TUPLE:
        print_type_list(", ", tl);
        break;

    case TYPE_UNION:
        print_type_list(" | ", tl);
        break;

    case TYPE_POINTER:
        print_type(pr->get_subtype());
        *out << "*";
        break;

    case TYPE_REFERENCE:
        print_type(pr->get_subtype());
        *out << "&";
        break;

    case TYPE_PARENTHESIS:
        *out << "(";
        print_type(pr->get_subtype());
        *out << ")";

    default:
        break;
    }
}

void CppPrinter::print_type_list(std::string oper, TypeList* tlist) {
    int i;

    for (i = 0; i < tlist->types_count() - 1; ++i) {
        print_type(tlist->get_type(i));
        *out << oper;
    }

    print_type(tlist->get_type(i));
}

void CppPrinter::print_statement(Statement* statement) {
    int kind = statement->get_kind();

    switch (kind) {
    case STMT_COMPOUND:
        print_compound_statement((CompoundStatement*) statement);
        break;

    case STMT_WHILE:
        print_while_statement((WhileStatement*) statement);
        break;

    case STMT_FOR:
        print_for_statement((ForStatement*) statement);
        break;

    case STMT_FOREACH:
        print_foreach_statement((ForStatement*) statement);
        break;

    case STMT_EXPRESSION:
        print_expression_statement((ExpressionStatement*) statement);
        break;

    case STMT_IF:
    case STMT_ELIF:
    case STMT_ELSE:
        print_branch_statement((BranchStatement*) statement);
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

void CppPrinter::print_while_statement(WhileStatement* statement) {
    print_indentation();
    *out << "while (";
    print_expression(statement->get_condition());
    *out << ") {\n";
    indent();
    print_compound_statement(statement->get_statements());
    dedent();
    print_indentation();
    *out << "}\n";
}

void CppPrinter::print_for_statement(ForStatement* statement) {
    print_indentation();

    *out << "for (";
    print_expression(statement->get_initialization());
    *out << "; ";
    print_expression(statement->get_condition());
    *out << "; ";
    print_expression(statement->get_increment());
    *out << ")";

    *out << " {\n";
    indent();
    print_compound_statement(statement->get_statements());
    dedent();
    print_indentation();
    *out << "}\n";
}

void CppPrinter::print_foreach_statement(ForStatement* statement) {
    BinOp* expr;
    Expression* init;
    Expression* end;
    Expression* var;
    print_indentation();
    *out << "for (";

    expr = (BinOp*) statement->get_condition();

    var = expr->get_left();
    init = ((BinOp*) expr->get_right())->get_left();
    end = ((BinOp*) expr->get_right())->get_right();

    *out << var->get_type()->to_cpp() << ' ';
    print_expression(var);
    *out << " = ";
    print_expression(init);
    *out << "; ";
    print_expression(var);
    *out << " < ";
    print_expression(end);
    *out << "; ++";
    print_expression(var);

    *out << ") {\n";
    indent();
    print_compound_statement(statement->get_statements());
    dedent();
    print_indentation();
    *out << "}\n";
}

void CppPrinter::print_branch_statement(BranchStatement* statement) {
    int kind;

    kind = statement->get_kind();
    print_indentation();

    switch (kind) {
    case STMT_IF:
        *out << "if (";
        print_expression(statement->get_condition());
        *out << ") {\n";
        indent();
        print_statement(statement->get_true_statements());
        dedent();
        print_indentation();
        *out << "}\n";

        if (statement->get_false_statements() != nullptr) {
            print_statement(statement->get_false_statements());
        }

        break;

    case STMT_ELIF:
        *out << "else if (";
        print_expression(statement->get_condition());
        *out << ") {\n";
        indent();
        print_statement(statement->get_true_statements());
        dedent();
        print_indentation();
        *out << "}\n";

        if (statement->get_false_statements() != nullptr) {
            print_statement(statement->get_false_statements());
        }

        break;

    case STMT_ELSE:
        *out << "else {\n";
        indent();
        print_statement(statement->get_true_statements());
        dedent();
        print_indentation();
        *out << "}\n";

        break;

    }
}

void CppPrinter::print_jump_statement(std::string op, JumpStatement* statement) {
    print_indentation();
    *out << op;

    if (statement->get_expression() != nullptr) {
        *out << ' ';
        print_expression(statement->get_expression());
    }

    *out << ";\n";
}

void CppPrinter::print_compound_statement(CompoundStatement* statement) {
    for (int i = 0; i < statement->statements_count(); ++i) {
        print_statement(statement->get_statement(i));
    }
}

void CppPrinter::print_expression_statement(ExpressionStatement* statement) {
    print_indentation();
    print_expression(statement->get_expression());
    *out << ";\n";
}

void CppPrinter::print_variable_declaration(VarDeclaration* decl) {
    Variable* var;

    var = decl->get_variable();
    print_indentation();

    if (var->get_type() != nullptr) {
        print_type(var->get_type());
    } else {
        // FIXME
        std::cout << __FILE__ << ' ' << __LINE__ << ' ';
        std::cout << "type should not be null\n";
        exit(0);
    }

    *out << ' ' << var->get_unique_name();

    if (decl->get_expression() != nullptr) {
        *out << " = ";
        print_expression(decl->get_expression());
    }

    *out << ";\n";
}

void CppPrinter::print_expression(Expression* expression) {
    if (expression == nullptr) return;

    int kind = expression->get_kind();
    BinOp* bin = (BinOp*) expression;
    UnOp* un = (UnOp*) expression;
    Literal* literal = (Literal*) expression;
    ExpressionList* exprlist = (ExpressionList*) expression;

    switch (kind) {
    case EXPR_ID:
        print_identifier((Identifier*) expression);
        break;

    case EXPR_THIS:
        print_this((ThisExpression*) expression);
        break;

    case EXPR_SCOPE:
        print_expression(bin->get_left());
        *out << "::";
        print_expression(bin->get_right());
        break;

    case EXPR_GLOBAL_SCOPE:
        *out << "::";
        print_expression(un->get_expression());
        break;

    case EXPR_ASSIGN:
        if (bin->get_initial_assign()) {
            print_type(bin->get_left()->get_type());
            *out << ' ';
        }

        print_binop("=", bin);
        break;

    case EXPR_SPECIAL_ASSIGN:
        print_binop(":=", bin);
        break;

    case EXPR_SRL_ASSIGN:
        print_binop(">>>=", bin);
        break;

    case EXPR_SRA_ASSIGN:
        print_binop(">>=", bin);
        break;

    case EXPR_SLL_ASSIGN:
        print_binop("<<=", bin);
        break;

    case EXPR_TIMES_ASSIGN:
        print_binop("*=", bin);
        break;

    case EXPR_PLUS_ASSIGN:
        print_binop("+=", bin);
        break;

    case EXPR_MODULO_ASSIGN:
        print_binop("%=", bin);
        break;

    case EXPR_MINUS_ASSIGN:
        print_binop("-=", bin);
        break;

    case EXPR_INTEGER_DIVISIO_ASSIGN:
        print_binop("//=", bin);
        break;

    case EXPR_DIVISION_ASSIGN:
        print_binop("/=", bin);
        break;

    case EXPR_BITWISE_NOT_ASSIGN:
        print_binop("~=", bin);
        break;

    case EXPR_BITWISE_OR_ASSIGN:
        print_binop("|=", bin);
        break;

    case EXPR_BITWISE_XOR_ASSIGN:
        print_binop("^=", bin);
        break;

    case EXPR_BITWISE_AND_ASSIGN:
        print_binop("&=", bin);
        break;

    case EXPR_PLUS:
        print_binop("+", bin);
        break;

    case EXPR_MINUS:
        print_binop("-", bin);
        break;

    case EXPR_TIMES:
        print_binop("*", bin);
        break;

    case EXPR_DIVISION:
        print_binop("/", bin);
        break;

    case EXPR_INTEGER_DIVISION:
        print_binop("//", bin);
        break;

    case EXPR_MODULO:
        print_binop("%", bin);
        break;

    case EXPR_POWER:
        print_binop("**", bin);
        break;

    case EXPR_BITWISE_OR:
        print_binop("|", bin);
        break;

    case EXPR_BITWISE_XOR:
        print_binop("^", bin);
        break;

    case EXPR_BITWISE_AND:
        print_binop("&", bin);
        break;

    case EXPR_SLL:
        print_binop("<<", bin);
        break;

    case EXPR_SRL:
        print_binop(">>>", bin);
        break;

    case EXPR_SRA:
        print_binop(">>", bin);
        break;

    case EXPR_INCLUSIVE_RANGE:
        print_binop("..", bin);
        break;

    case EXPR_EXCLUSIVE_RANGE:
        print_binop("...", bin);
        break;

    case EXPR_LOGICAL_OR:
        print_binop("or", bin);
        break;

    case EXPR_LOGICAL_OR_OPER:
        print_binop("||", bin);
        break;

    case EXPR_LOGICAL_AND:
        print_binop("and", bin);
        break;

    case EXPR_LOGICAL_AND_OPER:
        print_binop("&&", bin);
        break;

    case EXPR_EQ:
        print_binop("==", bin);
        break;

    case EXPR_NE:
        print_binop("!=", bin);
        break;

    case EXPR_LT:
        print_binop("<", bin);
        break;

    case EXPR_GT:
        print_binop(">", bin);
        break;

    case EXPR_LE:
        print_binop("<=", bin);
        break;

    case EXPR_GE:
        print_binop(">=", bin);
        break;

    case EXPR_IN:
        print_binop("in", bin);
        break;

    case EXPR_NOT_IN:
        print_binop("not in", bin);
        break;

    case EXPR_LOGICAL_NOT_OPER:
        print_unop("!", un);
        break;

    case EXPR_LOGICAL_NOT:
        print_unop("not ", un);
        break;

    case EXPR_ADDRESS_OF:
        print_unop("&", un);
        break;

    case EXPR_DEREFERENCE:
        print_unop("*", un);
        break;

    case EXPR_BITWISE_NOT:
        print_unop("~", un);
        break;

    case EXPR_UNARY_MINUS:
        print_unop("-", un);
        break;

    case EXPR_DOUBLE_DOLAR:
        print_double_dolar(un);
        break;

    case EXPR_UNARY_PLUS:
        print_unop("+", un);
        break;

    case EXPR_PRE_INC:
        print_unop("++", un);
        break;

    case EXPR_PRE_DEC:
        print_unop("--", un);
        break;

    case EXPR_POS_INC:
        print_unop("++", un, false);
        break;

    case EXPR_POS_DEC:
        print_unop("--", un, false);
        break;

    case EXPR_SIZEOF:
        print_unop("!", un);
        break;

    case EXPR_PARENTHESIS:
        *out << "(";
        print_expression(un->get_expression());
        *out << ")";
        break;

    case EXPR_CALL:
        print_expression(bin->get_left());
        print_expression(bin->get_right());
        break;

    case EXPR_INDEX:
        print_expression(bin->get_left());
        *out << "[";
        print_expression(bin->get_right());
        *out << "]";
        break;

    case EXPR_ARROW:
        print_expression(bin->get_left());
        *out << "->";
        print_expression(bin->get_right());
        break;

    case EXPR_DOT:
        print_dot(bin);
        break;

    case EXPR_LITERAL_BOOL:
    case EXPR_LITERAL_INTEGER:
    case EXPR_LITERAL_FLOAT:
    case EXPR_LITERAL_DOUBLE:
    case EXPR_LITERAL_CHAR:
    case EXPR_LITERAL_STRING:
        print_literal(literal);
        break;

    case EXPR_LITERAL_SYMBOL:
        print_literal_symbol(literal);
        break;

    case EXPR_LITERAL_NULL:
        *out << "nullptr";
        break;

    case EXPR_TUPLE:
        print_expression_list("(", ")", exprlist);
        break;

    case EXPR_LIST:
        print_expression_list("[", "]", exprlist);
        break;

    case EXPR_ARRAY:
        print_expression_list("{", "}", exprlist);
        break;

    case EXPR_ARGS:
        print_expression_list("(", ")", exprlist);
        break;

    case EXPR_FOR_INIT:
    case EXPR_FOR_INC:
        print_expression_list("", "", exprlist);
        break;

    case EXPR_HASH:
    case EXPR_HASH_RAW:
        print_hash(exprlist);
        break;

    case EXPR_FUNCTION:
        print_function_expression((FunctionExpression*) expression);
        break;

    case EXPR_NEW:
        print_new_expression((NewExpression*) expression);
        break;

    case EXPR_DELETE:
        print_delete_expression(un);
        break;

    case EXPR_DELETE_ARRAY:
        print_delete_array_expression(un);
        break;

    case EXPR_CAST:
        print_cast_expression((CastExpression*) expression);
        break;
    }
}

void CppPrinter::print_dot(BinOp* bin) {
    print_expression(bin->get_left());

    if (bin->get_left()->get_type()->get_kind() == TYPE_POINTER) {
        *out << "->";
    } else {
        *out << ".";
    }

    print_expression(bin->get_right());
}

void CppPrinter::print_binop(std::string oper, BinOp* bin) {
    print_expression(bin->get_left());
    *out << " " << oper << " ";
    print_expression(bin->get_right());
}

void CppPrinter::print_unop(std::string oper, UnOp* un, bool before) {
    if (before) {
        *out << oper;
    }

    print_expression(un->get_expression());

    if (!before) {
        *out << oper;
    }
}

void CppPrinter::print_double_dolar(UnOp* un) {
    Literal* l = (Literal*) un->get_expression();
    std::string s(l->get_lexeme());
    s[0] = ' ';
    s[s.size() - 1] = ' ';
    *out << s;
}

void CppPrinter::print_identifier(Identifier* id) {
    *out << id->get_unique_name();
}

void CppPrinter::print_this(ThisExpression* expr) {
    *out << "this";
}

void CppPrinter::print_literal(Literal* literal) {
    if (literal->get_kind() == EXPR_LITERAL_STRING) {
        std::string s = literal->get_lexeme();
        s[0] = '"';
        s[s.size() - 1] = '"';
        *out << s;
    } else {
        *out << literal->get_lexeme();
    }
}

void CppPrinter::print_literal_symbol(Literal* literal) {
    if (symbol_map.count(literal->get_lexeme()) == 0) {
        symbol_map[literal->get_lexeme()] = symbol_counter;
        symbol_counter++;
    }

    *out << "sym" << symbol_map.at(literal->get_lexeme());
}

void CppPrinter::print_expression_list(std::string begin, std::string end, ExpressionList* tuple) {
    int i;
    *out << begin;

    for (i = 0; i < tuple->expressions_count() - 1; ++i) {
        print_expression(tuple->get_expression(i));
        *out << ", ";
    }
        
    print_expression(tuple->get_expression(i));
    *out << end;
}

void CppPrinter::print_hash(ExpressionList* hash) {
    BinOp* pair;
    int i;

    if (hash->get_kind() == EXPR_HASH) {
        *out << "{";
    }

    for (i = 0; i < hash->expressions_count() - 1; ++i) {
        pair = (BinOp*) hash->get_expression(i);
        print_expression(pair->get_left());
        *out << ": ";
        print_expression(pair->get_right());
        *out << ", ";
    }

    pair = (BinOp*) hash->get_expression(i);
    print_expression(pair->get_left());
    *out << ": ";
    print_expression(pair->get_right());

    if (hash->get_kind() == EXPR_HASH) {
        *out << "}";
    }
}

void CppPrinter::print_function_expression(FunctionExpression* function) {
    int i;
    Function* f = function->get_function();

    *out << "|";

    if (f->parameters_count() > 0) {
        for (i = 0; i < f->parameters_count() - 1; ++i) {
            *out << f->get_parameter(i)->get_name();
            *out << ", ";
        }

        *out << f->get_parameter(i)->get_name();
    }

    *out << "| {\n";

    indent();
    print_compound_statement(f->get_statements());
    dedent();

    print_indentation();
    *out << "}";
}

void CppPrinter::print_new_expression(NewExpression* expr) {
    *out << "new ";
    print_type(expr->get_new_type());

    if (expr->has_arguments()) {
        print_expression_list("(", ")", expr->get_arguments());
    }
}

void CppPrinter::print_delete_expression(UnOp *expr) {
    *out << "delete ";
    print_expression(expr->get_expression());
}

void CppPrinter::print_delete_array_expression(UnOp *expr) {
    *out << "delete[] ";
    print_expression(expr->get_expression());
}

void CppPrinter::print_cast_expression(CastExpression* expr) {
    *out << "((";
    print_type(expr->get_cast_type());
    *out << ") ";
    print_expression(expr->get_expression());
    *out << ")";
}

void CppPrinter::print_class_constructors(Class* klass) {
    for (int i = 0; i < klass->constructors_count(); ++i) {
        print_indentation();
        *out << klass->get_cpp_name();

        Function* f = klass->get_method(i);
        print_parameters(f);
        print_indentation();
        *out << "    " << f->get_cpp_name() << '(';

        if (f->parameters_count() > 0) {
            int j = 0;
            for (j = 0; j < f->parameters_count() - 1; ++j) {
                *out << "p" << j << "_" << f->get_parameter(j)->get_name() << ", ";
            }
            *out << "p" << j << "_" << f->get_parameter(j)->get_name();
        }

        *out << ");\n";
        print_indentation();
        *out << "}\n\n";
    }
}

void CppPrinter::print_class_destructor(Class* klass) {
    Function* f = klass->get_destructor();

    if (f == nullptr) {
        return;
    }

    print_indentation();
    *out << '~' << klass->get_cpp_name();
    print_parameters(f);
    print_indentation();
    *out << "    " << f->get_cpp_name() << '(';

    if (f->parameters_count() > 0) {
        int j = 0;
        for (j = 0; j < f->parameters_count() - 1; ++j) {
            *out << "p" << j << "_" << f->get_parameter(j)->get_name() << ", ";
        }
        *out << "p" << j << "_" << f->get_parameter(j)->get_name();
    }

    *out << ");\n";
    print_indentation();
    *out << "}\n";
}

void CppPrinter::indent() {
    indent_c++;
}

void CppPrinter::dedent() {
    indent_c--;
}

void CppPrinter::print_indentation() {
    for (int i = 0; i < indent_c; ++i) {
        *out << "    ";
    }
}

void CppPrinter::generate_symbols(std::stringstream& out) {
    std::map<std::string, int>::iterator it;

    for (it = symbol_map.begin(); it != symbol_map.end(); ++it) {
        out << "sym sym" << it->second << " = \"" << it->first << "\";\n";
    }

    out << "\n";
}
